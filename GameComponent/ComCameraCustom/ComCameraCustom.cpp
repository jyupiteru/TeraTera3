/**
 * @file ComCameraCustom.cpp
 * @author jupiter ()
 * @brief ComCameraCustomの実装が書かれたcpp
 */
#include "ComCameraCustom.h"

void ComCameraCustom::Ready()
{
    m_playerTransform = GameObject::Find("player")->m_transform;
    m_camera = m_gameObject->GetComponent<ComCamera>();
    //m_camera->m_otherTransform = m_playerTransform;
}

void ComCameraCustom::Update()
{
    m_gameObject->m_transform->m_worldPosition.SetValue(m_playerTransform->m_worldPosition.GetValue());
}