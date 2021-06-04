#include "ComTimer.h"

void ComTimer::Init()
{
    m_comText = m_gameObject->GetComponent<Com2DText>();

    if (m_comText == nullptr)
    {
        m_comText = m_gameObject->AddComponent<Com2DText>();
    }

    //m_comText->m_text = std::to_string(m_maxCount);
    //m_comText->m_flagSetRate.SetValue(true);
}

//================================================================================================
//================================================================================================

void ComTimer::Update()
{
    //m_comText->m_text = std::to_string(m_maxCount);
}
