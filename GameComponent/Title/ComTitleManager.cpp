/**
 * @file COmTitleManager.cpp
 * @author jupiter ()
 * @brief
 * @version 1.0
 * @date 2021-08-23
 */

#include "ComTitleManager.h"

void ComTitleManager::Update()
{

	{
		auto title = GameObject::Find("title");
		title->m_transform->m_color.AddValue(0, 0, 0, 0.001f);
	}

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
}