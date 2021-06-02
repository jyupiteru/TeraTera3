/**
 * @file ObjectGenerator.cpp
 * @author jupiter
 * @brief ObjectGeneratorが記載されたcpp
 */

#include "ObjectGenerator.h"
#include "GameObject.h"
#include "../ComponentPackages/DefaultPackages.h"
#include "../Components/Behavior/Com2DText/Com2DText.h"
#include "../Components/System/ComCamera/ComCamera.h"
#include "../../../ThirdParty/ImGui/imgui.h"
#include "../../ImGuiSystem/CImGuiManager/CImGuiHelper/CImGuiHelper.h"
#include "../../ImGuiSystem/ImGuiHelperFunctions.h"
#include "../../WindowsSystem/CDirectInput.h"
#include "../../Timer/CTimer.h"
#include "../../EventSystem/CEventSystem.h"

ObjectGenerator *ObjectGenerator::m_instance = nullptr;

#include <string>

void ObjectGenerator::Init()
{
	m_cTagManager = std::make_unique<CTagManager>();
}

void ObjectGenerator::Uninit()
{
	for (auto itr = m_pListAllObject.begin(); itr != m_pListAllObject.end(); ++itr)
	{ //ここで全削除するとエラー（abort）の発生原因になるので別場所で削除
		this->DestroyInGenerator(itr->second.get());
	}

	EraseObject();
	//shared_ptrのポインタを開放
	m_pListAllObject.clear();
	m_pListObjectName.clear();
	m_cTagManager.reset();
}

//================================================================================================
//================================================================================================

void ObjectGenerator::Update()
{
	m_updateCounter = 0;
	m_updateTime = CTimer::GetInstance().GetProgressTime();

	auto [camera_x, camera_y, camera_z] = m_pCameraObject->m_transform->m_worldPosition.GetValue();

	for (auto &itr : m_listObjectForUpdate)
	{
		if (m_pListAllObject.contains(itr.second))
		{
			auto obj = &m_pListAllObject[itr.second];
			//objIDからオブジェクトポインタを取得してアクティブか確認する
			if ((*obj)->m_activeFlag.GetValue() == true)
			{
				m_updateCounter++;
				(*obj)->Update();
			}

			{ //描画用の情報をセットする場所
				//todo ここの処理が重いようなら計算場所、方法を見直し
				auto [r, g, b, a] = (*obj)->m_transform->m_color.GetValue();
				auto [pos_x, pos_y, pos_z] = (*obj)->m_transform->m_worldPosition.GetValue();
				auto distance = (camera_x - pos_x) * (camera_x - pos_x) +
								(camera_y - pos_y) * (camera_y - pos_y) +
								(camera_z - pos_z) * (camera_z - pos_z);
				distance = sqrt(distance);
				SetObjectDrawingOrder(**obj, distance);
			}
		}
	}

	ResetUpdateList();
	EraseObject();

	m_updateTime = CTimer::GetInstance().GetProgressTime() - m_updateTime;
}

//================================================================================================
//================================================================================================

void ObjectGenerator::Draw()
{
	m_drawTime = CTimer::GetInstance().GetProgressTime();
	m_drawCounter = 0;

#ifdef FLAG_DEBUGLOG_CHECKNOTFRUSTUMTYPE
	std::string notfrustumname;
#endif
	//フラスタムカリング用の方程式
	tagPLANE frustum[6];

	//todo このGetComponentの計算、重くなっているなら処理の変更
	ComCamera *camera = m_pCameraObject->GetComponent<ComCamera>();
	camera->GetPLANEForFrustum(frustum);

	//ここらにフラスタムカリングの処理を追加

	//todo ここら辺vector、tuple、sortで短縮できるはずなので時間ができたら修正すること
	//各レイヤーを透明でない->半透明の順に描画
	for (auto &itr : m_listObjectForDraw)
	{
		//各レイヤーの描画
		for (auto &itr2 : itr.second)
		{
#ifdef FLAG_DEBUGLOG_CHECKNOTFRUSTUMTYPE
			notfrustumname = "layer:" + std::to_string(itr.first) + " name:";
#endif
			if (itr2.first == 0)
			{ //透明でないオブジェクトの描画
				for (auto &itr3 : itr2.second)
				{
					//描画優先度順に描画
					for (auto &itr4 : itr3.second)
					{
						//オブジェクトを削除する処理を追加しており消えている可能性があるので
						if (m_pListAllObject.contains(itr4.second))
						{
							auto obj = &m_pListAllObject[itr4.second];
							//objIDからオブジェクトポインタを取得してアクティブか確認する
							if ((*obj)->m_activeFlag.GetValue() == true)
							{
								//フラスタムカリングを行い不要なものの削除
								//フラスタムの対象のオブジェクトかつうちにある
								if ((camera->IsInFrustum(*(*obj).get(), frustum) &&
									 ((*obj)->m_typeObject == E_TYPE_OBJECT::MODEL3D ||
									  (*obj)->m_typeObject == E_TYPE_OBJECT::IMAGE ||
									  (*obj)->m_typeObject == E_TYPE_OBJECT::BILLBOARD)) ||

									//フラスタムの対象外のオブジェクトである
									((*obj)->m_typeObject != E_TYPE_OBJECT::MODEL3D &&
									 (*obj)->m_typeObject != E_TYPE_OBJECT::IMAGE &&
									 (*obj)->m_typeObject != E_TYPE_OBJECT::BILLBOARD))
								{

#ifdef FLAG_DEBUGLOG_CHECKNOTFRUSTUMTYPE
									if (obj->m_typeObject != E_TYPE_OBJECT::MODEL3D &&
										obj->m_typeObject != E_TYPE_OBJECT::IMAGE &&
										obj->m_typeObject != E_TYPE_OBJECT::BILLBOARD)
									{
										notfrustumname += obj->m_objectName + ",";
									}
#endif

									(*obj)->Draw();
									m_drawCounter++;
								}
							}
							SetObjectUpdateOrder(*(*obj).get());
						}
					}
				}
			}
			else
			{ //半透明オブジェクトの描画
				for (auto itr3 = itr2.second.rbegin(); itr3 != itr2.second.rend(); itr3++)
				{
					//描画優先度順に描画
					for (auto &itr4 : itr3->second)
					{
						//オブジェクトを削除する処理を追加しており消えている可能性があるので
						if (m_pListAllObject.contains(itr4.second))
						{
							auto obj = &m_pListAllObject[itr4.second];
							//objIDからオブジェクトポインタを取得してアクティブか確認する
							if ((*obj)->m_activeFlag.GetValue() == true)
							{
								//フラスタムカリングを行い不要なものの削除
								//フラスタムの対象のオブジェクトかつうちにある
								if ((camera->IsInFrustum(*(*obj).get(), frustum) &&
									 ((*obj)->m_typeObject == E_TYPE_OBJECT::MODEL3D ||
									  (*obj)->m_typeObject == E_TYPE_OBJECT::IMAGE ||
									  (*obj)->m_typeObject == E_TYPE_OBJECT::BILLBOARD)) ||

									//フラスタムの対象外のオブジェクトである
									((*obj)->m_typeObject != E_TYPE_OBJECT::MODEL3D &&
									 (*obj)->m_typeObject != E_TYPE_OBJECT::IMAGE &&
									 (*obj)->m_typeObject != E_TYPE_OBJECT::BILLBOARD))
								{
#ifdef FLAG_DEBUGLOG_CHECKNOTFRUSTUMTYPE
									if (obj->m_typeObject != E_TYPE_OBJECT::MODEL3D &&
										obj->m_typeObject != E_TYPE_OBJECT::IMAGE &&
										obj->m_typeObject != E_TYPE_OBJECT::BILLBOARD)
									{
										notfrustumname += obj->m_objectName + ",";
									}
#endif
									(*obj)->Draw();
									m_drawCounter++;
								}
							}
							SetObjectUpdateOrder(*(*obj).get());
						}
					}
				}
			}
#ifdef FLAG_DEBUGLOG_CHECKNOTFRUSTUMTYPE
			DebugLog::Draw(notfrustumname);
#endif
		}
	}

	ResetDrawList();

	m_drawTime = CTimer::GetInstance().GetProgressTime() - m_drawTime;
}

//================================================================================================
//================================================================================================

void ObjectGenerator::Create()
{
	m_instance = new ObjectGenerator();
}

//================================================================================================
//================================================================================================

void ObjectGenerator::Delete(bool _flag)
{
	if (_flag)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

//================================================================================================
//================================================================================================

ObjectGenerator &ObjectGenerator::GetInstance()
{
	return *m_instance;
}

//================================================================================================
//================================================================================================

void ObjectGenerator::EraseObject()
{
	if (m_listEraseObject.size() > 0)
	{
		//todo 同時に削除するオブジェクトが増えると重くなると思うので対処する

		//こうするのは子オブジェクトを持っている場合ループ中にそれも破棄リストに入り増えてバグるため
		std::vector<int> nowlist;
		nowlist = m_listEraseObject;

		m_listEraseObject.clear();

		for (auto &objid : nowlist)
		{ //消す処理

			auto obj = &m_pListAllObject[objid];

			//リストからオブジェクト番号を削除
			m_pListObjectName.erase((*obj)->m_objectName);

			CCollision3DSystem::GetInstance().EraseCollisionObject(objid);

			//オブジェクトをリストから削除
			m_pListAllObject[objid].reset();
			m_pListAllObject.erase(objid);
		}
		nowlist.clear();
	}
}

//================================================================================================
//================================================================================================

void ObjectGenerator::ChangeScene(std::string_view scenename)
{
	//範囲for文を使いオブジェクトリストから各オブジェクト削除
	for (auto itr = m_pListAllObject.begin(); itr != m_pListAllObject.end(); ++itr)
	{
		//削除待ちのコンポーネントがあるなら削除(次のInit時に何かしらのエラーが出る可能性があるので)
		itr->second->EraseComponent();

		//このオブジェクトはシーンをまたぐかどうか
		if (!itr->second->m_isStraddle)
		{ //このオブジェクトはシーンをまたがない
			DestroyInGenerator(itr->first);
		}
	}
	EraseObject();
	GameObject::m_nowSceneName = scenename.data();
}

//================================================================================================
//================================================================================================

GameObject *const ObjectGenerator::AddObjectInGenerator(E_TYPE_OBJECT type)
{
	int count = 0;

	//オブジェクトの番号を自動生成するために既に登録されている名前なら変える
	while (1)
	{
		std::string name = "GameObject(" + std::to_string(count) + ")";
		//この名前が既に存在しているか確認
		if (!m_pListObjectName.contains(name))
		{
			return AddObjectInGenerator(name, type);
		}
		count++;
	}
}

//================================================================================================
//================================================================================================

GameObject *const ObjectGenerator::AddObjectInGenerator(std::string_view name, E_TYPE_OBJECT type)
{
	//すでに生成されている名前ではないか確認
	if (m_pListObjectName.contains(name.data()) == 0)
	{
		//コンストラクタをprivateにしたときにshared_ptrを作成する裏技
		struct impl : GameObject
		{
			friend ObjectGenerator;
			impl() : GameObject() {}
		};
		//オブジェクトを生成
		std::unique_ptr<GameObject> tmpobj = std::make_unique<impl>();
		int objid = tmpobj->m_objID;

		//一時的にnullptrを入れswapでお互いを交換している
		m_pListAllObject[objid] = nullptr;
		m_pListAllObject[objid].swap(tmpobj);

		auto obj = &m_pListAllObject[objid];

		(*obj)->m_objectName = name;
		E_DEFAULT_UPDATEPRIORITY priority;

		(*obj)->m_typeObject = type;

		//初期コンポーネント追加する処理
		switch (type)
		{
		case E_TYPE_OBJECT::IMAGE:
			(*obj)->AddPackage<Package3DTexObject>();
			priority = E_DEFAULT_UPDATEPRIORITY::IMAGE;
			break;

		case E_TYPE_OBJECT::BILLBOARD:
			(*obj)->AddPackage<Package3DBillBoard>();
			priority = E_DEFAULT_UPDATEPRIORITY::BILLBOARD;
			break;

		case E_TYPE_OBJECT::UI:
			(*obj)->AddPackage<PackageUITexture>();
			priority = E_DEFAULT_UPDATEPRIORITY::UI;
			break;

		case E_TYPE_OBJECT::MODEL3D:
			(*obj)->AddPackage<Package3DModelObjectAssimp>();
			priority = E_DEFAULT_UPDATEPRIORITY::MODEL;
			break;

		case E_TYPE_OBJECT::SYSTEM:
			priority = E_DEFAULT_UPDATEPRIORITY::SYSTEM;
			break;

		case E_TYPE_OBJECT::NONE:
		default:
			priority = E_DEFAULT_UPDATEPRIORITY::NONE;
			break;
		}
		(*obj)->m_objectUpdatePriority.SetValue(static_cast<int>(priority));

		//リストに名前とIDを登録
		m_pListObjectName[name.data()] = objid;

		SetObjectUpdateOrder(*m_pListAllObject[objid].get());

		if (name == "camera")
		{
			m_pCameraObject = m_pListAllObject[objid].get();
		}

		return m_pListAllObject[objid].get();
	}
	return nullptr;
}

GameObject *const ObjectGenerator::FindInGenerator(int objid)
{
	if (m_pListAllObject.contains(objid) == 1)
	{
		return m_pListAllObject[objid].get();
	}
	return nullptr;
}

//================================================================================================
//================================================================================================

GameObject *const ObjectGenerator::FindInGenerator(std::string_view name)
{
	//この引数が名前リストに登録されているか検索 C++20の機能なので注意それより前はcountを使用！
	if (m_pListObjectName.contains(name.data()) == 1)
	{
		auto objkey = m_pListObjectName[name.data()];
		return m_pListAllObject[objkey].get();
	}
	return nullptr;
}

//================================================================================================
//================================================================================================

bool ObjectGenerator::DestroyInGenerator(GameObject *obj)
{
	if (obj != nullptr)
	{
		//親オブジェクトが設定されているので先にそれの解除
		if (obj->m_pParentObject != nullptr)
		{
			obj->m_pParentObject->m_pListChildObject.erase(obj->m_objID);
			obj->m_pParentObject->m_pListChildObjectName.erase(obj->m_objectName);
			obj->m_pParentObject = nullptr;
		}
		m_listEraseObject.push_back(obj->m_objID);
		return true;
	}
	return false;
}

//================================================================================================
//================================================================================================

bool ObjectGenerator::DestroyInGenerator(std::string name)
{
	auto id = m_pListObjectName[name];
	return DestroyInGenerator(m_pListAllObject[id].get());
}

//================================================================================================
//================================================================================================

void ObjectGenerator::ImGuiDraw(int windowid)
{
	//各オブジェクトの内容描画処理、選択処理

	//描画内容
	//オブジェクト描画数
	//オブジェクトの総数

	ImGui::BulletText("ObjectTotal : %d", m_pListAllObject.size());

	ImGui::BulletText("ObjectUpdateTotal : %u", m_updateCounter);
	ImGui::BulletText("UpdateTime : %0.7f", m_updateTime);

	ImGui::BulletText("ObjectDrawTotal : %u", m_drawCounter);
	ImGui::BulletText("DrawTime : %0.7f", m_drawTime);
}

//================================================================================================
//================================================================================================

void ObjectGenerator::ImGuiDraw_Objects(int windowid)
{
	//名前、現在表示するもの、フラグ

	std::string hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "objectgenerator_imgui", "hideobject");

	//何も選択されたことがないのなら何もなくす
	if (hide_objectname == "None")
	{
		hide_objectname = "";
	}

	//強制的にキャスト(エラー出るかも)
	auto input_hidename = (char *)hide_objectname.c_str();

	//描画時のフィルター用 TABを押しながら打っていることを確認
	struct TextFilters
	{
		static int FilterImGuiLetters(ImGuiInputTextCallbackData *data)
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
	HelpMarker((const char *)u8"検索したいオブジェクト名を入れると絞り込み可能、TABを押しながら打つこと");

	//登録と取得のし直し
	CImGuiHelper::SetWindowDisplayContent(windowid, "objectgenerator_imgui", "hideobject", input_hidename);
	hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "objectgenerator_imgui", "hideobject");

	std::string selectobject = CImGuiHelper::GetWindowDisplayContent(windowid, "objectgenerator_imgui", "selectobject");

	if (ImGui::BeginListBox("SelectObject"))
	{
		//全オブジェクトぶん回し
		for (auto &itr : m_pListObjectName)
		{

			//選択されているものか 同じならtrue
			const bool is_selected = (selectobject == itr.first);

			//隠すフラグがたっていないかor絞り込みは機能していないか
			if (itr.first.find(hide_objectname) != std::string::npos || hide_objectname == "")
			{
				//1コメの引数が表示する内容、２個目が選択されたか？
				if (ImGui::Selectable((const char *)itr.first.c_str(), is_selected))
				{
					selectobject = itr.first;
					//登録内容の更新
					CImGuiHelper::SetWindowDisplayContent(windowid, "objectgenerator_imgui", "selectobject", itr.first);
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

	//すでに何か選択しているか(対象は存在するか)
	if (m_pListObjectName.contains(selectobject))
	{
		//リストから次のオブジェクトのidを取得
		unsigned int select_objectid = m_pListObjectName[selectobject];

		//次のオブジェクトのImGuiDrawを実行
		m_pListAllObject[select_objectid]->ImGui_Draw(windowid);
	}
}