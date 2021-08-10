﻿/**
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

    m_flagIsStandGround = false;
}

void ComPlayerMove::OnTriggerStay3D(GameObject *obj)
{
    //地面に触れたので各種初期化
    m_flagIsStandGround = true;

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

    //ジャンプ中でない
    if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_START &&
        m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_NOW &&
        m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_END &&
        m_flagNextAnimation != E_FLAG_PLAYERSTATE::READY)
    {

        DirectX::XMFLOAT3 vec = {0.0f, 0.0f, 0.0f};

        //移動処理判定フラグ
        bool flagmove = false;

        auto [angle_x, angle_y, angle_z] = m_gameObject->m_transform->m_angle.GetValue();

        float nowangle = 0.0f;

        { //移動処理
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
                flagmove = true;
                nowangle += 0.0f;
            }
            else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
            {
                flagmove = true;
                nowangle += 180.0f;
            }
        }

        //移動したか？
        if (flagmove)
        {
            m_gameObject->m_transform->m_angle.SetValue(0, nowangle, 0);

            //速度追加

            vec.z = movespeed;

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

        m_gameObject->m_transform->m_vector.SetValue(vec.x, vec.y, vec.z);

        //ジャンプをしたか？
        if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
        {
            m_flagNextAnimation = E_FLAG_PLAYERSTATE::JUMP_START;
            m_animationflag = false;
            m_fallCount = 0.0f;
        }
    }

    //ジャンプ中か
    if (m_flagNextAnimation == E_FLAG_PLAYERSTATE::JUMP_START ||
        m_flagNextAnimation == E_FLAG_PLAYERSTATE::JUMP_NOW)
    {
        auto pos_y = MathJump();
        m_gameObject->m_transform->m_worldPosition.AddValue(0.0f, pos_y, 0.0f);

        m_gameObject->m_transform->m_vector.SetValue(0.0f, 0.0f, movespeed);
        m_animationflag = true;
    }
    //ジャンプ中ではないのに地面についていないか(つまり落下中)
    else if (m_flagIsStandGround == false)
    {
        //ジャンプしていないのに地面の上にいない=落下している
        float speed = 10.0f * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());
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
    const float angle = 90.0f;
    const float gravity = 9.8f;

    static float lasttime = 0.0f;

    //y軸は斜方投射で実装
    //float pos_y;
    //pos_y = m_jumpSpeed * sin(angle * (DirectX::XM_PI / 180));
    //pos_y -= 0.5f * gravity * m_fallCount * m_fallCount;

    //float pos2_y = 0.0f;

    ////前フレームでのジャンプ元からの高さの計算
    //pos2_y = m_jumpSpeed * sin(angle * (DirectX::XM_PI / 180));
    //pos2_y -= 0.5f * gravity * lasttime * lasttime;


    //std::string sentence = "pos_y" + std::to_string(pos_y);
    //CDebugLog::GetInstance().Draw(sentence);

    //sentence = "pos2_y" + std::to_string(pos2_y);
    //CDebugLog::GetInstance().Draw(sentence);

    //pos_y = pos_y - pos2_y;

    ////pos_y *= CTimer::GetInstance().m_deltaTime.GetValue();
    //sentence = "last" + std::to_string(pos_y);
    //CDebugLog::GetInstance().Draw(sentence);
    ////CDebugLog::GetInstance().Draw(std::to_string(pos_y));

    //lasttime = m_fallCount;

    m_fallCount += CTimer::GetInstance().m_deltaTime.GetValue();

    float pos_y = m_jumpSpeed - 9.8f * m_fallCount;

    pos_y *= static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    return pos_y;
}