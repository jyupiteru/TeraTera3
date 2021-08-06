/**
 * @file ComPlayerMove.cpp
 * @author jupiter ()
 * @brief ComPlayerMoveの実装が記載されているcpp
 */

#include "ComPlayerMove.h"
#include "../ComFlowManager/ComFlowManager.h"

void ComPlayerMove::Init()
{
}

void ComPlayerMove::Uninit()
{
    m_comAnimation = nullptr;
}

void ComPlayerMove::Ready()
{
    m_comAnimation = m_gameObject->GetComponent<Com3DAnimationAssimp>();
    m_comAnimation->ChangeAnimation("falling");

    m_pComFlowManager = GameObject::Find("GameManager")->GetComponent<ComFlowManager>();
}

void ComPlayerMove::Update()
{
    //移動処理
    GetKeyBoard();

    //アニメーションの変更
    ChangeAnimation();

    m_fallCount += CTimer::GetInstance().m_deltaTime.GetValue();
    m_flagIsStandGround = false;
}

void ComPlayerMove::OnTriggerStay3D(GameObject *obj)
{
    //地面に触れたので各種初期化
    m_flagIsStandGround = true;
    m_fallCount = 0.0f;

    //ゲームを開始する
    if (m_flagNextAnimation == E_FLAG_PLAYERSTATE::READY)
    {
        m_pComFlowManager->ChangeFlow(E_FLOW::GAME);
    }

    //ジャンプ中なら終わらせる
    if (m_flagNextAnimation == E_FLAG_PLAYERSTATE::JUMP_NOW || m_flagNextAnimation == E_FLAG_PLAYERSTATE::READY)
    {
        m_flagNextAnimation = E_FLAG_PLAYERSTATE::JUMP_END;
        m_animationflag = false;
    }
}

void ComPlayerMove::GetKeyBoard()
{
    //１秒当たりのスピード
    float movespeed = m_moveSpeed * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    //1秒あたりの回転量
    float rotaionspeed = m_rotateSpeed * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    float vector_z = 0.0f;
    float angle_y = 0.0f;

    //ジャンプ中でない
    if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_START &&
        m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_NOW &&
        m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_END)
    {
        //回転するか
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
        {
            angle_y = rotaionspeed;
        }
        else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
        {
            angle_y = -rotaionspeed;
        }

        //移動するか
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
        {
            vector_z = movespeed;
            if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::RUNING)
            {
                //走るアニメーションをセット
                m_flagNextAnimation = E_FLAG_PLAYERSTATE::RUNING;
                m_animationflag = false;
            }
        }
        else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
        {
            vector_z = -movespeed;
            if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::RUNING)
            {
                //走るアニメーションをセット
                m_flagNextAnimation = E_FLAG_PLAYERSTATE::RUNING;
                m_animationflag = false;
            }
        }
        //前フレームは走っていたのに入力ナシなら
        else if (m_flagNextAnimation == E_FLAG_PLAYERSTATE::RUNING)
        {
            m_flagNextAnimation = E_FLAG_PLAYERSTATE::IDLE;
            m_animationflag = false;
        }

    }

    //ジャンプをしているか
    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
    {
        m_flagNextAnimation = E_FLAG_PLAYERSTATE::JUMP_START;
        m_animationflag = false;
    }

    //それぞれをセット
    m_gameObject->m_transform->m_vector.SetValue(0.0f, 0.0f, vector_z);
    m_gameObject->m_transform->m_angle.AddValue(0.0f, angle_y, 0.0f);

    //ジャンプ中か
    if (m_flagNextAnimation == E_FLAG_PLAYERSTATE::JUMP_START ||
        m_flagNextAnimation == E_FLAG_PLAYERSTATE::JUMP_NOW)
    {
        auto vec_y = MathJump();
        vec_y *= CTimer::GetInstance().m_deltaTime.GetValue();
        m_gameObject->m_transform->m_worldPosition.AddValue(0.0f, vec_y, 0.0f);

        m_gameObject->m_transform->m_vector.SetValue(0.0f, 0.0f, movespeed);
        m_animationflag = true;
    }
    //ジャンプ中ではないのに地面についていないか(つまり落下中)
    else if (m_flagIsStandGround == false)
    {
        //ジャンプしていないのに地面の上にいない=落下している
        float speed = m_jumpSpeed * 2.0f * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());
        m_gameObject->m_transform->m_vector.SetValue(0.0f, -speed, 0.0f);
        m_animationflag = true;

        //準備段階でない(準備だったら前に出てしまうのでのぞく)
        if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::READY)
        {
            m_flagNextAnimation = E_FLAG_PLAYERSTATE::FALL;
        }
    }
}

void ComPlayerMove::ChangeAnimation()
{
    //現在のアニメーションは終了しているかorフラグは消えたか
    if (m_comAnimation->IsFinish() || m_animationflag == false)
    {
        //各種アニメーションに変更
        switch (m_flagNextAnimation)
        {
        case E_FLAG_PLAYERSTATE::IDLE:
            m_comAnimation->ChangeAnimation("idle");
            break;

        case E_FLAG_PLAYERSTATE::RUNING:
            m_comAnimation->ChangeAnimation("run");
            break;

        case E_FLAG_PLAYERSTATE::JUMP_START:
            m_comAnimation->ChangeAnimation("jumpup");
            m_flagNextAnimation = E_FLAG_PLAYERSTATE::JUMP_NOW;
            break;

        case E_FLAG_PLAYERSTATE::FALL:
        case E_FLAG_PLAYERSTATE::READY:
        case E_FLAG_PLAYERSTATE::JUMP_NOW:

            //ここの条件変更は地面にOnTriggerで変更する
            m_comAnimation->ChangeAnimation("falling");

            break;

        case E_FLAG_PLAYERSTATE::JUMP_END:
            m_comAnimation->ChangeAnimation("jumpdown");
            m_flagNextAnimation = E_FLAG_PLAYERSTATE::IDLE;

            //地面についているので初期状態に戻す
            // m_flagNextAnimation = E_FLAG_PLAYERSTATE::IDLE;
            break;
        }
        m_animationflag = true;
    }
}

float ComPlayerMove::MathJump()
{
    //y軸は斜方投射で実装

    const float angle = 60.0f;

    const float gravity = 9.8f;

    //=-gt+v0sinθ
    float vec_y = -gravity * m_fallCount + m_jumpSpeed * sin(angle * DirectX::XM_PI / 180);

    return vec_y;
}