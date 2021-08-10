/**
 * @file ComTimer.cpp
 * @author jupiter ()
 * @brief ComTimerの実装部分が記述されているcpp
 */

#include "ComTimer.h"

void ComTimer::Ready()
{
    m_pComText = m_gameObject->GetComponent<Com2DText>();

    //テキストはすでに読み込んであるか
    if (m_pComText == nullptr)
    {
        m_pComText = m_gameObject->AddComponent<Com2DText>();
    }
    m_pComText->m_text = std::to_string(m_maxCount);
}

void ComTimer::UpdateTime()
{
    m_nowCount += static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    //カウントが1.0秒超えるor現在の残り秒数が0秒以上で更新する
    if (m_nowCount >= 1.0f && m_maxCount >= 0)
    {
        m_maxCount -= 1;
        m_nowCount -= 1.0f;
        m_pComText->m_text = std::to_string(m_maxCount);
    }
}