
#include "ComPlayer.h"

void ComPlayer::Init()
{
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
    float movespeed = 3.0f;
    movespeed *= CTimer::GetInstance().m_deltaTime.GetValue();

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