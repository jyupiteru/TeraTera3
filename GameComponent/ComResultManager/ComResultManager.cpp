/**
 * @file ComResultManager.cpp
 * @author jupiter ()
 * @brief ComResultManagerクラスの実装が書かれたcpp
 */

#include "ComResultManager.h"
#include "../ComDataManager/ComDataManager.h"

void ComResultManager::Init()
{
}

//================================================================================================
//================================================================================================

void ComResultManager::Ready()
{
    //子オブジェクトの生成と座標設定
    GameObject *highscoretext = m_gameObject->AddChildObject("HighScoreText", E_TYPE_OBJECT::UI);
    highscoretext->m_transform->m_size.SetValue(450, 200, 1);
    highscoretext->m_transform->m_worldPosition.SetValue(0, 50, 0);
    highscoretext->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    highscoretext->RemoveComponent<Com2DTexture>();
    m_comhighScoreText = highscoretext->AddComponent<Com2DText>();
    m_comhighScoreText->m_flagSetRate.SetValue(true);

    GameObject *text = m_gameObject->AddChildObject("ScoreText", E_TYPE_OBJECT::UI);
    text->m_transform->m_size.SetValue(300, 200, 1);
    text->m_transform->m_worldPosition.SetValue(0, 0, 0);
    text->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    text->RemoveComponent<Com2DTexture>();
    m_comScoreText = text->AddComponent<Com2DText>();
    m_comScoreText->m_flagSetRate.SetValue(true);

    MathScore();
}

//================================================================================================
//================================================================================================

void ComResultManager::Update()
{
}

//================================================================================================
//================================================================================================

void ComResultManager::MathScore()
{
    float maxtime = ComDataManager::GetInstance().m_maxTime.GetValue();
    float timerate = ComDataManager::GetInstance().m_nowCount.GetValue();

    //全体のどれくらいまで行ったか計算
    timerate /= maxtime;

    //最終得点の計算とその何割かを計算
    maxtime *= 10;
    m_score = static_cast<int>(maxtime * timerate);

    m_comScoreText->m_text = "Score : " + std::to_string(m_score);

    if (m_score > ComDataManager::GetInstance().m_highScore.GetValue())
    {
        ComDataManager::GetInstance().m_highScore.SetValue(m_score);
    }
    m_comhighScoreText->m_text = "High Score : " + std::to_string(ComDataManager::GetInstance().m_highScore.GetValue());
}
