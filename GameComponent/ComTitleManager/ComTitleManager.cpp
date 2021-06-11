/**
 * @file ComTitleManager.cpp
 * @author jupiter ()
 * @brief ComTitleManagerクラスの実装が書かれたcpp
 */

#include "ComTitleManager.h"
#include "../ComDataManager/ComDataManager.h"

void ComTitleManager::Init()
{
}

//================================================================================================
//================================================================================================

void ComTitleManager::Ready()
{
    //子オブジェクトの生成と座標設定
    GameObject *highscoretext = m_gameObject->AddChildObject("HighScoreText", E_TYPE_OBJECT::UI);
    highscoretext->m_transform->m_size.SetValue(450, 200, 1);
    highscoretext->m_transform->m_worldPosition.SetValue(0, -100, 0);
    highscoretext->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    highscoretext->RemoveComponent<Com2DTexture>();
    m_comhighScoreText = highscoretext->AddComponent<Com2DText>();
    m_comhighScoreText->m_flagSetRate.SetValue(true);
    m_comhighScoreText->m_text = "High Score : " + std::to_string(ComDataManager::GetInstance().m_highScore.GetValue());

    GameObject *space = m_gameObject->AddChildObject("PushSpace", E_TYPE_OBJECT::UI);
    space->m_transform->m_size.SetValue(250, 200, 1);
    space->m_transform->m_worldPosition.SetValue(0, -200, 0);
    space->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    space->RemoveComponent<Com2DTexture>();
    auto textspace = space->AddComponent<Com2DText>();
    textspace->m_flagSetRate.SetValue(true);
    textspace->m_text = "Push SpaceKey";
}

//================================================================================================
//================================================================================================

void ComTitleManager::Update()
{
}
