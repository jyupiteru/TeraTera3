/**
 * @file ComResultManager.cpp
 * @author jupiter ()
 * @brief ComResultManagerクラスの実装が書かれたcpp
 */

#include "ComResultManager.h"
#include "../ComDataManager/ComDataManager.h"

void ComResultManager::Init()
{
    MathScore();
}

//================================================================================================
//================================================================================================

void ComResultManager::Ready()
{
    int coinscore = ComDataManager::GetInstance().m_shotScore.GetValue();

    //各種スコア表示用のオブジェクトの生成とコンポーネント等の設定
    GameObject *cointext = m_gameObject->AddChildObject("CoinScoreText", E_TYPE_OBJECT::UI);
    cointext->m_transform->m_size.SetValue(450, 200, 1);
    cointext->m_transform->m_worldPosition.SetValue(0, 90, 0);
    cointext->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    cointext->RemoveComponent<Com2DTexture>();
    m_comCoinScoreText = cointext->AddComponent<Com2DText>();
    m_comCoinScoreText->m_flagSetRate.SetValue(true);
    m_comCoinScoreText->m_text = "CoinScore : " + std::to_string(coinscore);

    GameObject *timetext = m_gameObject->AddChildObject("TimeScoreText", E_TYPE_OBJECT::UI);
    timetext->m_transform->m_size.SetValue(450, 200, 1);
    timetext->m_transform->m_worldPosition.SetValue(0, 45, 0);
    timetext->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    timetext->RemoveComponent<Com2DTexture>();
    m_comTimeScoreText = timetext->AddComponent<Com2DText>();
    m_comTimeScoreText->m_flagSetRate.SetValue(true);
    m_comTimeScoreText->m_text = "TimeScore : " + std::to_string(m_timeScore);

    GameObject *text = m_gameObject->AddChildObject("TotalScoreText", E_TYPE_OBJECT::UI);
    text->m_transform->m_size.SetValue(450, 200, 1);
    text->m_transform->m_worldPosition.SetValue(0, 0, 0);
    text->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    text->RemoveComponent<Com2DTexture>();
    m_comScoreText = text->AddComponent<Com2DText>();
    m_comScoreText->m_flagSetRate.SetValue(true);
    m_comScoreText->m_text = "TotalScore : " + std::to_string(m_timeScore + coinscore);

    GameObject *highscoretext = m_gameObject->AddChildObject("HighScoreText", E_TYPE_OBJECT::UI);
    highscoretext->m_transform->m_size.SetValue(450, 200, 1);
    highscoretext->m_transform->m_worldPosition.SetValue(0, -45, 0);
    highscoretext->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    highscoretext->RemoveComponent<Com2DTexture>();
    m_comhighScoreText = highscoretext->AddComponent<Com2DText>();
    m_comhighScoreText->m_flagSetRate.SetValue(true);
    m_comhighScoreText->m_text = "High Score : " + std::to_string(ComDataManager::GetInstance().m_highScore.GetValue());
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
    int coinscore = ComDataManager::GetInstance().m_shotScore.GetValue();

    float maxtime = ComDataManager::GetInstance().m_maxTime.GetValue();
    float timerate = ComDataManager::GetInstance().m_nowCount.GetValue();

    //全体のどれくらいまで行ったか計算
    timerate /= maxtime;

    //最終得点の計算とその何割かを計算
    maxtime *= 200;
    m_timeScore = static_cast<int>(maxtime * timerate);

    //ハイスコアを上回っているか？
    if (m_timeScore + coinscore > ComDataManager::GetInstance().m_highScore.GetValue())
    {
        ComDataManager::GetInstance().m_highScore.SetValue(m_timeScore + coinscore);
    }
    ComDataManager::GetInstance().SaveScore();
}
