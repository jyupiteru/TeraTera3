/**
 * @file ComPlayerTitle.cpp
 * @author jupiter ()
 * @brief ComPlayerTitleの実装が書かれたcpp
 */

#include "ComPlayerTitle.h"

void ComPlayerTitle::Ready()
{
    m_pComAnimation = m_gameObject->GetComponent<Com3DAnimationAssimp>();
}

void ComPlayerTitle::Uninit()
{
    m_pComAnimation = nullptr;
}

void ComPlayerTitle::Update()
{
    //アニメーションは終わったか
    if (m_pComAnimation->IsFinish())
    {
        //次のアニメーションに変更
        ChangeAnimation();
    }
}

void ComPlayerTitle::ChangeAnimation()
{
    //さっきまで再生していたアニメーションは再生順が最後か
    if (m_nowAnimation < 4)
    {
        m_nowAnimation++;
    }
    else
    { //最初に戻る
        m_nowAnimation = 0;
    }

    //次のアニメーションに変更
    switch (m_nowAnimation)
    {
    case 2:
        m_pComAnimation->ChangeAnimation("neck");
        break;

    case 4:
        m_pComAnimation->ChangeAnimation("arm");
        break;

    case 0:
    default:
        m_pComAnimation->ChangeAnimation("idle");
        break;
    }
}