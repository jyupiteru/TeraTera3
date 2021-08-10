/**
 * @file ComPlayerMove.cpp
 * @author jupiter ()
 * @brief ComPlayerMoveの実装が記載されているcpp
 */

#include "ComPlayerMove.h"
#include "ComFlowManager.h"

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

    //プレイヤーと衝突した箱の座標、大きさを取得する
    auto [boxpos_x, boxpos_y, boxpos_z] = obj->m_transform->m_worldPosition.GetValue();
    auto [boxsize_x, boxsize_y, boxsize_z] = obj->m_transform->m_size.GetValue();
    auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue();
    auto [size_x, size_y, size_z] = m_gameObject->m_transform->m_size.GetValue();

    //箱にめり込んでいるかを計算して確認する
    if (pos_y - size_y / 2 <= boxpos_y + boxsize_y / 2)
    {//めり込んでいるので落下を続ける
        m_flagIsStandGround = false;
    }
    //ジャンプ中なら終わらせる
    else if (m_flagNextAnimation == E_FLAG_PLAYERSTATE::JUMP_NOW || m_flagNextAnimation == E_FLAG_PLAYERSTATE::READY)
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
    if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::READY)
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

            if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_START &&
                m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_NOW &&
                m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_END &&
                m_flagNextAnimation != E_FLAG_PLAYERSTATE::RUNING)
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

        //ジャンプをしたか？ && ジャンプ中でないか
        if (m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_START &&
            m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_NOW &&
            m_flagNextAnimation != E_FLAG_PLAYERSTATE::JUMP_END &&
            CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
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

    m_fallCount += CTimer::GetInstance().m_deltaTime.GetValue();
    float pos_y = m_jumpSpeed - 9.8f * m_fallCount;
    pos_y *= static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    return pos_y;
}