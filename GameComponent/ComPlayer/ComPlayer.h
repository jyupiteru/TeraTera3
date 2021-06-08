/**
 * @file ComPlayer.h
 * @author jupiter ()
 * @brief ComPlayerクラスの宣言を記述したヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief プレイヤーの状況の列挙型
 */
enum class E_PLAYERFLOW
{
    READY,
    GAME
};

/**
 * @brief プレイヤーの処理全般を管理しているコンポーネントクラス
 */
class ComPlayer : public ComponentBase
{
    /**
     * @brief 現在のプレイヤーの状況を管理する変数
     */
    E_PLAYERFLOW m_nowstate = E_PLAYERFLOW::READY;

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
};
