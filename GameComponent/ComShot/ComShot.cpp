/**
 * @file ComShot.cpp
 * @author jupiter ()
 * @brief ComShotの実装を記述したcpp
 */

#include "ComShot.h"
#include "../ComShotManager/ComShotManager.h"

void ComShot::UpdateShot()
{
    //弾の速度を設定
    auto [vec_x, vec_y, vec_z] = m_shotSpeed.GetValue();

    float deltatime = static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    vec_x *= deltatime;
    vec_y *= deltatime;
    vec_z *= deltatime;

    m_gameObject->m_transform->m_worldPosition.AddValue(vec_x, vec_y, vec_z);
    m_gameObject->m_transform->m_angle.AddValue(0, 60 * deltatime, 0);
}

//================================================================================================
//================================================================================================

void ComShot::OnTriggerStay3D(GameObject *obj)
{
    //プレイヤーに当たったか？
    if (//obj->m_objectName.find("Coin") != std::string::npos ||
        obj->m_objectName.find("Player") != std::string::npos)
    {
        ComShotManager::GetInstance().SetShotReadyList(this);
    }
}