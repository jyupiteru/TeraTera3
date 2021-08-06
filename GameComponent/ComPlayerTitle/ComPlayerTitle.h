/**
 * @file ComPlayerTitle.h
 * @author jupiter ()
 * @brief ComPlayerTitleの宣言が記載されたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief タイトルにおけるプレイヤーのアニメーションの遷移を行うコンポーネント
 */
class ComPlayerTitle : public ComponentBase
{
    /**
     * @brief 現在のアニメーションを管理する変数
     */
    int m_nowAnimation = 0;

    /**
     * @brief アニメーションコンポーネントのポインタ
     */
    Com3DAnimationAssimp *m_pComAnimation = nullptr;

public:
    ComPlayerTitle(){};
    ~ComPlayerTitle(){};

    void Ready() override;

    void Uninit() override;

    void Update() override;

private:
    /**
     * @brief アニメーションを変化させるさいに使用するメソッド
     */
    void ChangeAnimation();
};
