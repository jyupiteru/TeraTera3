/**
 * @file ComTimer.cpp
 * @author jupiter ()
 * @brief ComTimerの実装が書かれたcpp
 */

#include "ComTimer.h"

ComTimer *ComTimer::m_instance = nullptr;

void ComTimer::Init()
{
    m_comText = m_gameObject->GetComponent<Com2DText>();

    if (m_comText == nullptr)
    {
        m_comText = m_gameObject->AddComponent<Com2DText>();
    }

    m_maxTimeCount.SetValue(60.0f);
    m_comText->m_flagSetRate.SetValue(true);
    m_instance = this;
}

//================================================================================================
//================================================================================================

void ComTimer::Ready()
{
    m_timeCount = static_cast<int>(m_maxTimeCount.GetValue());
    m_comText->m_text = std::to_string(m_timeCount);
    m_nowCount.SetValue(m_maxTimeCount.GetValue() - m_maxTimeCount.GetValue());
}

//================================================================================================
//================================================================================================

void ComTimer::Update()
{
    //カウントの更新
    m_milliCount += static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    //カウントが1.0秒超えるor現在の残り秒数が0秒以上で更新する
    if (m_milliCount >= 1.0f && m_timeCount >= 0)
    {
        //表示している秒数の更新
        m_timeCount -= 1;
        m_milliCount -= 1.0f;
        m_comText->m_text = std::to_string(m_timeCount);
        m_nowCount.SetValue(static_cast<float>(m_maxTimeCount.GetValue() - m_timeCount));
    }
}
