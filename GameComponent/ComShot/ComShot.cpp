
#include "ComShot.h"
#include "../ComShotManager/ComShotManager.h"

void ComShot::Update()
{
    auto [vec_x, vec_y, vec_z] = m_shotSpeed.GetValue();

    vec_x *= CTimer::GetInstance().m_deltaTime.GetValue();
    vec_y *= CTimer::GetInstance().m_deltaTime.GetValue();
    vec_z *= CTimer::GetInstance().m_deltaTime.GetValue();

    m_gameObject->m_transform->m_worldPosition.AddValue(vec_x, vec_y, vec_z);
}