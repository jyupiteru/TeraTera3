/**
 * @file COmTitleManager.cpp
 * @author jupiter ()
 * @brief
 * @version 1.0
 * @date 2021-08-23
 */

#include "ComTitleManager.h"
#include "../ComDataManager/ComDataManager.h"

void ComTitleManager::Ready()
{
	m_selectStageText = GameObject::Find("StageNum")->GetComponent<Com2DText>();
}

//================================================================================================
//================================================================================================

void ComTitleManager::Update()
{
	//タイトル画像を徐々に表示する
	float speed = 0.2f * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());
	m_titleImageObj->m_transform->m_color.AddValue(0, 0, 0, speed);

	//シーンの移動
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
	{
		CSceneManager::GetInstance().LoadScene("SceneGame1");
	}

	//ImGuiの表示切替
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LSHIFT))
	{
		bool flag = CImGuiManager::GetInstance().m_flagSurvival.GetValue();
		CImGuiManager::GetInstance().m_flagSurvival.SetValue(!flag);
	}

	//ステージ選択の変更
	SelectStage();
}

//================================================================================================
//================================================================================================

void ComTitleManager::SelectStage()
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
	if (ComDataManager::GetInstance().m_mapsData.size() - 1 >= stagenum && stagenum >= 0)
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
		m_selectStageText->m_text = "Tutorial Stage";
	}
}