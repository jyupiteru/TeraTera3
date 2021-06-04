#include "ComTimer.h"

void ComTimer::Init()
{
    m_comText = m_gameObject->GetComponent<Com2DText>();

    if (m_comText == nullptr)
    {
        m_comText = m_gameObject->AddComponent<Com2DText>();
    }

    m_timeCount = static_cast<int>(m_maxTimeCount);
    m_comText->m_text = std::to_string(m_timeCount);
    m_comText->m_flagSetRate.SetValue(true);
}

//================================================================================================
//================================================================================================

void ComTimer::Update()
{
    m_milliCount += CTimer::GetInstance().m_deltaTime.GetValue();


    //カウントが1.0秒超えるor現在の残り秒数が0秒以上で更新する
    if (m_milliCount >= 1.0f && m_timeCount >= 0)
    {
        m_timeCount -= 1;
        m_milliCount -= 1.0f;
        m_comText->m_text = std::to_string(m_timeCount);
    }
}
