/**
 * @file ComPlayer.h
 * @author jupiter ()
 * @brief ComPlayerクラスの宣言を記述したヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once



/**
 * @brief プレイヤーの処理全般を管理しているコンポーネントクラス
 */
class ComPlayer : public ComponentBase
{
    /**
     * @brief プレイヤーの状況の列挙型
     */
    enum class E_PLAYERFLOW
    {
        READY,
        GAME,
        FALL
    };

    /**
     * @brief プレイヤーの現在の状態を制御するための列挙型
     */
    enum class E_FLAG_ANIMATIONPLAYER
    {
        IDLE,
        RUNING,
        JUMP_START,
        JUMP_NOW,
        JUMP_END,
    };

    E_FLAG_ANIMATIONPLAYER m_nowPlayerAnimation = E_FLAG_ANIMATIONPLAYER::JUMP_NOW;

    /**
     * @brief 現在のプレイヤーの状況を管理する変数
     */
    E_PLAYERFLOW m_nowstate = E_PLAYERFLOW::READY;

    /**
     * @brief このオブジェクトのアニメーションへのパス
     */
    Com3DAnimationAssimp *m_comAnimation = nullptr;

    /**
     * @brief 現在ブロックに触れているかを判定するフラグ
     */
    bool m_flagIsStandGround = false;

    /**
     * @brief アニメーションの遷移を管理するフラグ
     * @n trueで強制的にアニメーションを次に遷移
     */
    bool m_flagChangeAnimation = false;

    /**
     * @brief プレイヤーが何秒間地面から離れたかを数える変数
     */
    float m_fallCount = 0;

public:
    /**
     * @brief プレイヤーの移動スピード 初期10
     */
    CVector<float> m_playerSpeed;

public:
    ComPlayer(){};
    ~ComPlayer(){};

    void Init() override;
    void Uninit() override;
    void Ready() override;
    void Update() override;
    void OnTriggerStay3D(GameObject *obj);

private:
    /**
     * @brief プレイヤーの移動処理
     */
    void PlayerMove();

    /**
     * @brief アニメーションを変更する(常に通る)メソッド
     */
    void ChangeAnimation();
};
