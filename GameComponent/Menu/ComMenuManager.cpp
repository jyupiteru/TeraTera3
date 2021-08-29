/**
 * @file ComMenuManager.cpp
 * @author jupiter ()
 * @brief ComMenuManagerの実装を記載するcpp
 */

#include "ComMenuManager.h"
#include "../ComDataManager/ComDataManager.h"

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

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
	{
		CSceneManager::GetInstance().LoadScene("SceneGame1");
	}
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

	m_selectStageText->m_text = "Play ";

	//チュートリアルステージ用の分岐
	if (stagenum != 0)
	{
		m_selectStageText->m_text += "Stage : ";
		m_selectStageText->m_text += std::to_string(stagenum);
	}
	else
	{
		m_selectStageText->m_text += "Tutorial Stage";
	}
}