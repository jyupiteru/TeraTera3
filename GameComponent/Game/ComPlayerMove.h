/**
 * @file ComPlayerMove.h
 * @author jupiter ()
 * @brief ComPlayerMoveの宣言が乗っているヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComFlowManager;

/**
 * @brief プレイヤーの移動に関する処理を扱うコンポーネント
 */
class ComPlayerMove final : public ComponentBase
{
public:
    /**
     * @brief プレイヤーの現在の状態を制御するための列挙型
     */
    enum class E_FLAG_PLAYERSTATE
    {
        READY,
        IDLE,
        RUNING,
        JUMP_START,
        JUMP_NOW,
        JUMP_END,
        FALL
    };

    /**
     * @brief このオブジェクトのアニメーションへのパス
     */
    Com3DAnimationAssimp *m_comAnimation = nullptr;

    /**
     * @brief ジャンプ時の初速度
     */
    float m_jumpSpeed = 5.0f;

    /**
     * @brief 1秒当たりの回転量
     */
    float m_rotateSpeed = 160.0f;

    /**
     * @brief １秒当たりの移動量
     */
    float m_moveSpeed = 10.0f;

    /**
     * @brief アニメーションを全部させるというフラグ
     * @n falseで強制終了
     */
    bool m_animationflag = true;

private:
    /**
     * @brief 現在ブロックに触れているかを判定するフラグ
     */
    bool m_flagIsStandGround = false;

    /**
     * @brief プレイヤーが何秒間地面から離れたかを数える変数
     */
    float m_fallCount = 0;

    /**
     * @brief プレイヤーの次のアニメーションを保管する変数
     */
    ComPlayerMove::E_FLAG_PLAYERSTATE m_flagNextAnimation = E_FLAG_PLAYERSTATE::READY;

    /**
     * @brief 落下時のフラグを変更するのに使用
     */
    ComFlowManager *m_pComFlowManager = nullptr;

public:
    ComPlayerMove(){};

    ~ComPlayerMove(){};

    void Init() override;

    void Uninit() override;

    void Ready() override;

    void Update() override;

    void OnTriggerStay3D(GameObject *obj);

private:
    /**
     * @brief プレイヤーの移動関係の処理が入っているメソッド
     */
    void GetKeyBoard();

    /**
     * @brief アニメーションを変更する(常に通る)メソッド
     */
    void ChangeAnimation();

    /**
     * @brief ジャンプ時のy軸の移動量を計算するメソッド
     * @return float y軸移動量
     */
    float MathJump();
};
