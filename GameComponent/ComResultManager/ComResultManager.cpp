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
    GameObject *text = m_gameObject->AddChildObject("pointText", E_TYPE_OBJECT::UI);
    text->m_transform->m_size.SetValue(300, 200, 1);
    text->m_transform->m_worldPosition.SetValue(0, 0, 0);
    text->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
    text->RemoveComponent<Com2DTexture>();
    m_comText = text->AddComponent<Com2DText>();
    m_comText->m_flagSetRate.SetValue(true);

    MathPoint();
}

//================================================================================================
//================================================================================================

void ComResultManager::Update()
{
}

//================================================================================================
//================================================================================================

void ComResultManager::MathPoint()
{
    float maxtime = ComDataManager::GetInstance().m_maxTime.GetValue();
    float timerate = ComDataManager::GetInstance().m_nowCount.GetValue();

    //全体のどれくらいまで行ったか計算
    timerate /= maxtime;

    //最終得点の計算とその何割かを計算
    maxtime *= 10;
    m_point = static_cast<int>(maxtime * timerate);

    m_comText->m_text = "Point : " + std::to_string(m_point)+"\naaa";
}
