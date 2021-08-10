/**
 * @file ComStageFall.cpp
 * @author jupiter ()
 * @brief ComStageFallの実装が書かれたcpp
 */
#include "ComStageFall.h"

void ComStageFall::Init()
{
    m_count.SetValue(256.0f);
    m_decreaseSpeed.SetValue(150.0f);
}

void ComStageFall::Update()
{
    float count = m_count.GetValue();

    m_gameObject->m_transform->m_color.SetValue(256.0f, count, count, 1.0f);
    if (count < 0)
    {
        float speed = 1.0f * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());
        m_gameObject->m_transform->m_vector.AddValue(0, -speed, 0.0f);
        if (auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue(); pos_y < -100)
        {
            m_gameObject->m_activeFlag.SetValue(false);
        }
    }
}

void ComStageFall::OnTriggerStay3D(GameObject *obj)
{
    if (obj->m_objectName == "player")
    {
        const float speed = m_decreaseSpeed.GetValue();

        m_count.AddValue(-speed * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue()));
    }
}