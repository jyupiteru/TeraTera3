/**
 * @file ComLightPoint.cpp
 * @author jupiter ()
 * @brief ComLightPointクラスの実装が記載されたcpp
 */

#include "ComLightPoint.h"
#include "../../Shape/ComSphere/ComSphere.h"
#include "../ComTransform/ComTransform.h"
#include "../../System/ComLight/ComLight.h"

ComLightPoint *ComLightPoint::m_instance = nullptr;

void ComLightPoint::Init()
{
    if (m_instance == nullptr)
    {
        m_instance = this;
        m_range.SetValue(100.0f);

        m_gameObject->AddComponent<ComSphere>();
        m_gameObject->m_transform->m_size.SetValue(5.0, 5.0f, 5.0f);
    }
    else
    {
        m_gameObject->RemoveComponent(this);
    }
}

//================================================================================================
//================================================================================================

void ComLightPoint::Uninit()
{
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

ComLightPoint *ComLightPoint::GetInstance()
{
    return m_instance;
}