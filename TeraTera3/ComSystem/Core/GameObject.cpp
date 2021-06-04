﻿/**
 * @file GameObject.cpp
 * @author jupiter
 * @brief GameObjectクラスの実装の一部を記述したcpp
 * @version 1.0
 * @date 2020-08-07
 */

#include "GameObject.h"
#include "ObjectGenerator.h"

 //読み込みたくないけどこれ以外に循環参照を知らないので
#include "ComponentBase.h"
#include "../Components/Behavior/ComTransform/ComTransform.h"
#include "../../../ThirdParty/ImGui/imgui.h"
#include "../../ImGuiSystem/ImGuiHeaders.h"
#include "../../ImGuiSystem/ImGuiHelperFunctions.h"
#include "../../WindowsSystem/CDirectInput.h"
#include "../../EventSystem/CEventSystem.h"

int GameObject::m_classCount = 0;

std::string GameObject::m_nowSceneName;

void GameObject::Init()
{
	m_transform = this->AddComponent<ComTransform>();
	m_activeFlag.SetValue(true);
}

//================================================================================================
//================================================================================================

void GameObject::Uninit()
{
	//コンポーネントリストから各コンポーネント削除
	for (auto itr = m_pListComponent.begin(); itr != m_pListComponent.end(); itr++)
	{
		//削除町のリストに登録する(一括で削除する)
		RemoveComponent(itr->second.get());
	}

	EraseComponent();

	m_pListComponent.clear();

	//オブジェクトリストから各オブジェクト削除
	for (auto itr = m_pListChildObject.begin(); itr != m_pListChildObject.end(); itr++)
	{
		itr->second->m_pParentObject = nullptr;
		//ジェネレータから子オブジェクトを削除
		ObjectGenerator::GetInstance().DestroyInGenerator(itr->second->m_objectName);
	}
	m_pListChildObject.clear();

	//オブジェクトキーリストから各オブジェクト削除
	m_pListChildObjectName.clear();
}

//================================================================================================
//================================================================================================

void GameObject::Update()
{
	auto tmpreadylist = m_listComponentReady;
	m_listComponentReady.clear();
	//ここ参照にしたかったけどするとmake_pairで値をとられる？のでなしに
	for (auto& itr : tmpreadylist)
	{
		//Update前にRemoveで消えている可能性もあるので
		if (m_pListComponent.contains(itr))
		{
			m_pListComponent[itr]->Ready();
		}
	}
	tmpreadylist.clear();

	//自分のコンポーネントのUpdate回し
	for (const auto& itr : m_pListComponent)
	{
		//このコンポーネントはアクティブかどうか
		if (itr.second->m_enable.GetValue())
		{
			itr.second->Update();
		}
	}

	EraseComponent();
}

//================================================================================================
//================================================================================================

void GameObject::Draw()
{
	//自分のDraw回し
	for (auto& itr : m_pListComponent)
	{
		if (itr.second->m_enable.GetValue())
		{
			itr.second->Draw();
		}
	}
}

//================================================================================================
//================================================================================================

GameObject* const GameObject::AddChildObject(E_TYPE_OBJECT type)
{
	auto obj = ObjectGenerator::GetInstance().AddObjectInGenerator(type);
	//きちんと生成できているかの確認
	if (obj != nullptr)
	{
		obj->m_pParentObject = this;
		m_pListChildObjectName[obj->m_objectName] = obj->m_objID;
		m_pListChildObject[obj->m_objID] = obj;
	}
	return obj;
}

//================================================================================================
//================================================================================================

GameObject* const GameObject::AddChildObject(std::string name, E_TYPE_OBJECT type)
{
	auto obj = ObjectGenerator::GetInstance().AddObjectInGenerator(name, type);
	//きちんと生成できているかの確認
	if (obj != nullptr)
	{
		obj->m_pParentObject = this;
		m_pListChildObjectName[name] = obj->m_objID;
		m_pListChildObject[obj->m_objID] = obj;
	}
	return obj;
}

//================================================================================================
//================================================================================================

GameObject* const GameObject::FindChildObject(std::string key)
{
	if (m_pListChildObjectName.contains(key))
	{
		auto objid = m_pListChildObjectName[key];
		return m_pListChildObject[objid];
	}
	return nullptr;
}

//================================================================================================
//================================================================================================

GameObject* const GameObject::FindChildObject(int objid)
{
	//idから検索
	if (m_pListChildObject.contains(objid))
	{ //存在する
		return m_pListChildObject[objid];
	} //存在しない
	return nullptr;
}

//================================================================================================
//================================================================================================

bool GameObject::DestroyChild(const int objid)
{
	//リスト内に存在するかどうか
	if (m_pListChildObject.contains(objid))
	{
		//オブジェクト名を取得
		auto objname = m_pListChildObject[objid]->m_objectName;

		//リストからオブジェクト番号を削除
		m_pListChildObjectName.erase(objname);

		//子オブジェクトをリストから削除
		m_pListChildObject.erase(objid);

		//ジェネレータのほうから削除
		return ObjectGenerator::GetInstance().DestroyInGenerator(objid);
	}
	return false;
}

//================================================================================================
//================================================================================================

void GameObject::EraseComponent()
{
	if (m_listEraseComponent.size() > 0)
	{
		std::vector<std::type_index> eraselist = m_listEraseComponent;

		m_listEraseComponent.clear();

		for (auto& itr : eraselist)
		{
			CEventSystem::GetInstance().EraseComponentFromEvent(this, m_pListComponent[itr].get());
			m_pListComponent[itr]->Uninit();
			//コンポーネントの所有権を放棄（ほかでshared_ptrのままで保持しているとそっちは消えない）
			m_pListComponent[itr].reset();
			m_pListComponent.erase(itr);
		}
		eraselist.clear();
	}
}

//================================================================================================
//================================================================================================

GameObject* const GameObject::MakeNewObject(std::string_view name, E_TYPE_OBJECT type)
{
	return ObjectGenerator::GetInstance().AddObjectInGenerator(name, type);
}

//================================================================================================
//================================================================================================

GameObject* const GameObject::Find(int objid)
{
	return ObjectGenerator::GetInstance().FindInGenerator(objid);
}

//================================================================================================
//================================================================================================

GameObject* const GameObject::Find(std::string_view name)
{
	return ObjectGenerator::GetInstance().FindInGenerator(name);
}

//================================================================================================
//================================================================================================

void GameObject::ImGui_Draw(unsigned int windowid)
{
	//表示内容
	//id、コンポーネント総数
	//シーンをまたぐか
	//子オブジェクトの数
	//子オブジェクトの名前一覧(できれば絞り込み機能も)
	//各コンポーネントの詳細を見る機能

	if (ImGui::TreeNode("Object Details"))
	{
		ImGui_DrawObjectDetails(windowid);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Components"))
	{
		ImGui_DrawComponents(windowid);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Child Object"))
	{
		ImGui_DrawChiObjects(windowid);
		ImGui::TreePop();
	}
}

//================================================================================================
//================================================================================================

void GameObject::ImGui_DrawObjectDetails(unsigned int windowid)
{
	ImGui::BulletText("ObjectID : %d", m_objID);

	ImGui::BulletText("DrawLayer : %d", m_drawLayer.GetValue());

	//シーンをまたぐか
	if (m_isStraddle)
	{
		ImGui::BulletText("isStraddle : true");
	}
	else
	{
		ImGui::BulletText("isStraddle : false");
	}
	ImGui::SameLine();
	HelpMarker((const char*)u8"オブジェクトはシーンをまたぐか?");

	if (m_pParentObject != nullptr)
	{
		ImGui::BulletText("ParentName : %s", m_pParentObject->m_objectName.c_str());
	}
	else
	{
		ImGui::BulletText("ParentName : None");
	}

	ImGui::BulletText("UpdatePriority : %d", m_objectUpdatePriority.GetValue());
	ImGui::BulletText("DrawPriority : %d", m_objectDrawPriority.GetValue());
}

//================================================================================================
//================================================================================================

void GameObject::ImGui_DrawComponents(unsigned int windowid)
{
	ImGui::BulletText("TotalComponent : %d", m_pListComponent.size());

	//子オブジェクトを所持しているか
	if (m_pListComponent.size() > 0)
	{
		std::string hide_componentname = CImGuiHelper::GetWindowDisplayContent(windowid, "object" + m_objID, "hidecomponent");

		//何も選択されたことがないのなら何もなくす
		if (hide_componentname == "None")
		{
			hide_componentname = "";
		}

		//強制的にキャスト(エラー出るかも)
		auto input_hidename = (char*)hide_componentname.c_str();

		//描画時のフィルター用 TABを押しながら打っていることを確認
		struct TextFilters
		{
			static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
			{
				if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_TAB))
					return 0;
				return 1;
			}
		};

		//テキストを取得する
		ImGui::InputText("filter", input_hidename, sizeof(input_hidename),
			ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		//次のものを同じ行に描画する?
		ImGui::SameLine();
		HelpMarker((const char*)u8"検索したいコンポーネント名を入れると絞り込み可能、TABを押しながら打つこと");

		//登録と取得のし直し
		CImGuiHelper::SetWindowDisplayContent(windowid, "object" + m_objID, "hidecomponent", input_hidename);
		hide_componentname = CImGuiHelper::GetWindowDisplayContent(windowid, "object" + m_objID, "hidecomponent");

		unsigned int componentcount = 0;

		//全オブジェクトぶん回し
		for (auto& itr : m_pListComponent)
		{
			std::string componentname = &itr.first.name()[6];

			//検索文字列が存在しているかor絞り込みは機能していないか
			if (componentname.find(hide_componentname) != std::string::npos || hide_componentname == "")
			{
				if (ImGui::TreeNode(componentname.c_str()))
				{
					//対象コンポーネントのImGui_Drawを実行
					itr.second->ImGui_Draw(windowid);
					ImGui::TreePop();
				}
				componentcount++;
			}
		}

		//検索結果のコンポーネントが0の時
		if (componentcount == 0)
		{
			ImGui::Text("%s doesn't exist", hide_componentname.c_str());
		}
	}
}

//================================================================================================
//================================================================================================

void GameObject::ImGui_DrawChiObjects(unsigned int windowid)
{
	ImGui::BulletText("TotalChildObject : %d", m_pListChildObject.size());

	//子オブジェクトを所持しているか
	if (m_pListChildObjectName.size() > 0)
	{
		std::string hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "object" + m_objID, "hideobject");

		//何も選択されたことがないのなら何もなくす
		if (hide_objectname == "None")
		{
			hide_objectname = "";
		}

		//強制的にキャスト(エラー出るかも)
		auto input_hidename = (char*)hide_objectname.c_str();

		//描画時のフィルター用 TABを押しながら打っていることを確認
		struct TextFilters
		{
			static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
			{
				if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_TAB))
					return 0;
				return 1;
			}
		};

		//テキストを取得する
		ImGui::InputText("filter", input_hidename, sizeof(input_hidename),
			ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		//次のものを同じ行に描画する?
		ImGui::SameLine();
		HelpMarker((const char*)u8"検索したいオブジェクト名を入れると絞り込み可能、TABを押しながら打つこと");

		//登録と取得のし直し
		CImGuiHelper::SetWindowDisplayContent(windowid, "object" + m_objID, "hideobject", input_hidename);
		hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "object" + m_objID, "hideobject");

		std::string selectobject = CImGuiHelper::GetWindowDisplayContent(windowid, "object" + m_objID, "select_objectchildobject");

		//開始処理かつ子オブジェクトが存在している
		if (ImGui::BeginListBox("ChildObjects"))
		{
			//全オブジェクトぶん回し
			for (auto& itr : m_pListChildObjectName)
			{
				//選択されているものか 同じならtrue
				const bool is_selected = (selectobject == itr.first);

				//隠すフラグがたっていないかor絞り込みは機能していないか
				if (itr.first.find(hide_objectname) != std::string::npos || hide_objectname == "")
				{
					//1コメの引数が表示する内容、２個目が選択されたか？
					if (ImGui::Selectable((const char*)itr.first.c_str(), is_selected))
					{
						//登録内容の更新
						CImGuiHelper::SetWindowDisplayContent(windowid, "object" + m_objID, "select_objectchildobject", itr.first);
					}

					//選択されたか
					if (is_selected)
					{
						//選択されている場所をデフォルトにする
						ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndListBox();
		}

		//選択したオブジェクトがまだ残っているか
		if (m_pListChildObjectName.contains(selectobject))
		{
			//idを取得して内容を描画
			unsigned int objectid = m_pListChildObjectName[selectobject];
			m_pListChildObject[objectid]->ImGui_Draw(windowid);
		}
	}
}

//================================================================================================
//================================================================================================

bool GameObject::DestroyObject(GameObject* obj)
{
	//アドレスが無効じゃないか確認
	if (obj != nullptr)
	{
		//親オブジェクトは設定されているか
		if (obj->m_pParentObject)
		{ //親オブジェクトから削除
			return obj->m_pParentObject->DestroyChild(obj);
		}
		return ObjectGenerator::GetInstance().DestroyInGenerator(obj->m_objID);
	}
	return false;
}

//================================================================================================
//================================================================================================

bool GameObject::DestroyObject(int id)
{
	return DestroyObject(ObjectGenerator::GetInstance().FindInGenerator(id));
}

//================================================================================================
//================================================================================================

bool GameObject::DestroyObject(std::string_view name)
{
	return DestroyObject(ObjectGenerator::GetInstance().FindInGenerator(name));
}