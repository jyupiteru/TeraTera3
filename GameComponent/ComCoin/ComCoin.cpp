/**
 * @file ComCoin.cpp
 * @author jupiter ()
 * @brief ComCoinクラスの実装が記述されているcpp
 */

#include "ComCoin.h"
#include "../ComCoinManager/ComCoinManager.h"

void ComCoin::Update()
{
    //回転速度を設定して回転させる
    float rotatespeed = 100;

    rotatespeed *= static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    m_gameObject->m_transform->m_angle.AddValue(0, rotatespeed, 0);
}

//================================================================================================
//================================================================================================

void ComCoin::OnTriggerStay3D(GameObject *obj)
{
    //衝突したのはプレイヤーか?
    if (obj->m_objectName.find("Player") != std::string::npos)
    {
        ComCoinManager::GetInstance().SetWaitList(this, true);
    }
    else if (obj->m_objectName.find("Shot") != std::string::npos)
    {
        ComCoinManager::GetInstance().SetWaitList(this, false);
    }
}