/**
 * @file ComMenuManager.cpp
 * @author jupiter ()
 * @brief ComMenuManagerの実装を記載するcpp
 */

#include "ComMenuManager.h"
#include "../ComDataManager/ComDataManager.h"
#include "ComMenuSelectorMove.h"

void ComMenuManager::Init()
{
	{
		m_selecterObject = GameObject::MakeNewObject("Selector", E_TYPE_OBJECT::UI);
		m_selecterObject->GetComponent<Com2DTexture>()->LoadTexture("selector.png");
		m_selecterObject->m_transform->m_worldPosition.SetValue(0.0f, 100, 0.0f);
		m_selecterObject->m_transform->m_size.SetValue(630.0f, 50.0f, 1);
		m_selecterObject->m_transform->m_color.SetValue(256.0f, 256.0f, 256.0f, 1.0f);
		ComMenuSelectorMove *comselect = m_selecterObject->AddComponent<ComMenuSelectorMove>();
		comselect->m_widthMaxPlusSize = 30.0f;
		comselect->m_plusSizePerSecond = 30.0f;
	}

	{
		GameObject *custom = GameObject::MakeNewObject("CustomImage", E_TYPE_OBJECT::UI);
		custom->m_transform->m_worldPosition.SetValue(0.0f, -100.0f, 0.0f);
		custom->m_transform->m_size.SetValue(210.0f, 35.0f, 1.0f);
		custom->GetComponent<Com2DTexture>()->LoadTexture("Custom.png");
	}

	{ 
		GameObject *startgame = GameObject::MakeNewObject("StartGameImage", E_TYPE_OBJECT::UI);
		startgame->m_transform->m_worldPosition.SetValue(0.0f, 100.0f, 0.0f);
		startgame->m_transform->m_size.SetValue(240.0f, 35.0f, 1.0f);
		startgame->GetComponent<Com2DTexture>()->LoadTexture("StartGame.png");
	}

	{
		GameObject *stagenum = GameObject::MakeNewObject("StageNum", E_TYPE_OBJECT::UI);
		stagenum->m_transform->m_worldPosition.SetValue(100.0f, 200.0f, 0.0f);
		stagenum->m_transform->m_size.SetValue(500.0f, 100.0f, 1);
		stagenum->m_transform->m_color.SetValue(0.0f, 0.0f, 0.0f, 1.0f);

		stagenum->RemoveComponent<Com2DTexture>();
		m_selectStageText = stagenum->AddComponent<Com2DText>();
		m_selectStageText->m_text = "Stage : 1";
		m_selectStageText->m_flagSetRate.SetValue(true);
	}

	{ //選択しているステージの案内画像
		GameObject *selectstage = GameObject::MakeNewObject("SelectStageImage", E_TYPE_OBJECT::UI);
		selectstage->m_transform->m_worldPosition.SetValue(-250.0f, 200.0f, 0.0f);
		selectstage->m_transform->m_size.SetValue(350.0f, 35.0f, 1.0f);
		selectstage->GetComponent<Com2DTexture>()->LoadTexture("SelectStage.png");
	}

	{ //案内の画像
		GameObject *selectmenu = GameObject::MakeNewObject("SelectMenuImage", E_TYPE_OBJECT::UI);
		selectmenu->m_transform->m_worldPosition.SetValue(0.0f, -250.0f, 0.0f);
		selectmenu->m_transform->m_size.SetValue(703.0f, 31.0f, 1.0f);
		selectmenu->GetComponent<Com2DTexture>()->LoadTexture("EnterInMenu.png");
	}
}

//================================================================================================
//================================================================================================

void ComMenuManager::Ready()
{
	m_selectStageText = GameObject::Find("StageNum")->GetComponent<Com2DText>();
}

//================================================================================================
//================================================================================================

void ComMenuManager::Update()
{

	//ImGuiの表示切替
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LSHIFT) &&
		CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RSHIFT))
	{
		bool flag = CImGuiManager::GetInstance().m_flagSurvival.GetValue();
		CImGuiManager::GetInstance().m_flagSurvival.SetValue(!flag);
	}

	//ステージ選択の変更
	UpdateStageSelect();

	UpdateSelect();
}

//================================================================================================
//================================================================================================

void ComMenuManager::UpdateStageSelect()
{
	int stagenum = ComDataManager::GetInstance().m_stageNum.GetValue();

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHTARROW))
	{
		stagenum++;
	}
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFTARROW))
	{
		stagenum--;
	}

	//存在しているステージか？
	if (ComDataManager::GetInstance().m_stagesData.size() - 1 >= stagenum && stagenum >= 0)
	{
		ComDataManager::GetInstance().m_stageNum.SetValue(stagenum);
	}
	else
	{
		stagenum = ComDataManager::GetInstance().m_stageNum.GetValue();
	}

	//チュートリアルステージ用の分岐
	if (stagenum != 0)
	{
		m_selectStageText->m_text = "Stage : ";
		m_selectStageText->m_text += std::to_string(stagenum);
	}
	else
	{
		m_selectStageText->m_text = "Tutorial ";
	}
}

//================================================================================================
//================================================================================================

void ComMenuManager::UpdateSelect()
{
	float selectpos = 0.0f;
	std::tie(std::ignore, selectpos, std::ignore) = m_selecterObject->m_transform->m_worldPosition.GetValue();

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP))
	{
		selectpos = 100.0f;
	}
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN))
	{
		selectpos = -100.0f;
	}

	m_selecterObject->m_transform->m_worldPosition.SetValue(0.0f, selectpos, 0.0f);

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
	{
		//位置によって遷移するシーンの分岐
		if (selectpos == 100.0f)
		{
			CSceneManager::GetInstance().LoadScene("SceneGame1");
		}
	}
}