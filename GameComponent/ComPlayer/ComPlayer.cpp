/**
 * @file ComPlayer.cpp
 * @author jupiter ()
 * @brief ComPlayerクラスの実装を記述したcpp
 */

#include "ComPlayer.h"
#include "../ComGameManager/ComGameManager.h"

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
    m_comAnimation = m_gameObject->GetComponent<Com3DAnimationAssimp>();
    m_comAnimation->ChangeAnimation("falling");


    m_fallCount += CTimer::GetInstance().m_deltaTime.GetValue();
    m_flagIsStandGround = false;
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

        //地面についているか？
        if (m_flagIsStandGround == true)
        {
            PlayerMove();
        }
        else
        {
            m_nowstate = E_PLAYERFLOW::FALL;
            ComGameManager::GetInstance().m_nowGameState.SetValue(E_GAMEFLOW::GAMEOVER);
        }

        break;

    case E_PLAYERFLOW::FALL:

        if (m_fallCount < 4.0f)
        {
            m_fallCount += static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());
            m_nowPlayerAnimation = E_FLAG_ANIMATIONPLAYER::JUMP_NOW;
        }
        else
        {
            ComGameManager::GetInstance().m_nowGameState.SetValue(E_GAMEFLOW::END);
        }
        break;
    }
    //アニメーションの変更
    ChangeAnimation();
    m_flagIsStandGround = false;
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
        ComGameManager::GetInstance().m_nowGameState.SetValue(E_GAMEFLOW::GAME);
        m_nowPlayerAnimation = E_FLAG_ANIMATIONPLAYER::IDLE;
        m_flagChangeAnimation = true;
    }
    else
    {
        //あたったのは弾か?
        if (obj->m_objectName.find("Shot") != std::string::npos)
        {
            m_nowstate = E_PLAYERFLOW::FALL;
            //ゲームオーバー処理
            ComGameManager::GetInstance().m_nowGameState.SetValue(E_GAMEFLOW::GAMEOVER);
        }
    }
    m_flagIsStandGround = true;
}

//================================================================================================
//================================================================================================

void ComPlayer::PlayerMove()
{
    DirectX::XMFLOAT3 vec = { 0.0f, 0.0f, 0.0f };

    bool flagmove = false;

    auto [angle_x, angle_y, angle_z] = m_gameObject->m_transform->m_angle.GetValue();

    float nowangle = 0.0f;

    {//移動処理
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
        {
            flagmove = true;

            if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
            {
                nowangle = 45.0f;
            }
            else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
            {
                nowangle = 135.0f;
            }
            else
            {
                nowangle = 90.0f;
            }
        }
        else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
        {
            flagmove = true;

            if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
            {
                nowangle = -45.0f;
            }
            else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
            {
                nowangle = -135.0f;
            }
            else
            {
                nowangle = -90.0f;
            }
        }
        else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
        {
            nowangle += 0.0f;
            flagmove = true;
        }
        else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
        {
            flagmove = true;
            nowangle += 180.0f;
        }
    }

    //移動後
    if (flagmove == true)
    {
        m_gameObject->m_transform->m_angle.SetValue(0, nowangle, 0);

        //速度追加
        float movespeed = m_playerSpeed.GetValue();
        movespeed *= static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());
        vec.z = movespeed;

        //現在のアニメーションは移動ではないか？
        if (m_nowPlayerAnimation != E_FLAG_ANIMATIONPLAYER::RUNING)
        {
            m_flagChangeAnimation = true;
            m_nowPlayerAnimation = E_FLAG_ANIMATIONPLAYER::RUNING;
        }
    }
    else if (!flagmove && m_nowPlayerAnimation != E_FLAG_ANIMATIONPLAYER::IDLE)
    {
        m_flagChangeAnimation = true;
        m_nowPlayerAnimation = E_FLAG_ANIMATIONPLAYER::IDLE;
    }

    m_gameObject->m_transform->m_vector.SetValue(vec.x, vec.y, vec.z);
}

//================================================================================================
//================================================================================================

void ComPlayer::ChangeAnimation()
{
    //現在のアニメーションは終了しているかorフラグは消えたか
    if (m_comAnimation->IsFinish() || m_flagChangeAnimation == true)
    {
        //各種アニメーションに変更
        switch (m_nowPlayerAnimation)
        {
        case E_FLAG_ANIMATIONPLAYER::IDLE:
            m_comAnimation->ChangeAnimation("idle");
            break;

        case E_FLAG_ANIMATIONPLAYER::RUNING:
            m_comAnimation->ChangeAnimation("run");
            break;


        case E_FLAG_ANIMATIONPLAYER::JUMP_NOW:
            m_comAnimation->ChangeAnimation("falling");

            break;

        default:
            CDebugLog::GetInstance().Draw("Animation Bug");
            break;
        }
        m_flagChangeAnimation = false;
    }
}
