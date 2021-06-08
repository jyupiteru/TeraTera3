/**
 * @file ComPlayer.cpp
 * @author jupiter ()
 * @brief ComPlayerクラスの実装を記述したcpp
 */

#include "ComPlayer.h"

void ComPlayer::Init()
{
    m_playerSpeed.SetValue(10.0f);
}

//================================================================================================
//================================================================================================

void ComPlayer::Uninit()
{
}

//================================================================================================
//================================================================================================

void ComPlayer::Ready()
{
}

//================================================================================================
//================================================================================================

void ComPlayer::Update()
{
    switch (m_nowstate)
    {
    case E_PLAYERFLOW::READY:
        //落下処理
        m_gameObject->m_transform->m_vector.SetValue(0.0f, -0.5f, 0.0f);
        break;

    case E_PLAYERFLOW::GAME:
        PlayerMove();
        break;
    }
}

//================================================================================================
//================================================================================================

void ComPlayer::OnTriggerStay3D(GameObject *obj)
{
    //READY時に衝突する=最初の落下が終わったということなので更新
    if (m_nowstate == E_PLAYERFLOW::READY)
    {
        m_nowstate = E_PLAYERFLOW::GAME;
        m_gameObject->m_transform->m_vector.SetValue(0.0f, 0.0f, 0.0f);
    }
}

//================================================================================================
//================================================================================================

void ComPlayer::PlayerMove()
{
    float movespeed = m_playerSpeed.GetValue();
    movespeed *= static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    DirectX::XMFLOAT3 vec = {0.0f, 0.0f, 0.0f};

    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
    {
        vec.x = -movespeed;
    }
    else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
    {
        vec.x = movespeed;
    }
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
    {
        vec.z = movespeed;
    }
    else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
    {
        vec.z = -movespeed;
    }

    m_gameObject->m_transform->m_vector.SetValue(vec.x, vec.y, vec.z);
}