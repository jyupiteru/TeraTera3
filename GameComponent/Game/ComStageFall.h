/**
 * @file ComStageFall.h
 * @author jupiter ()
 * @brief ComStageFallの宣言が書かれたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief 一定時間床に触れると落ちる処理を追加するコンポーネント
 */
class ComStageFall : public ComponentBase
{
public:
    /**
     * @brief 床の初期体力
     */
    CVector<float> m_count;

    /**
     * @brief 床の秒間辺りの体力の減少量
     */
    CVector<float> m_decreaseSpeed;

    void Init() override;

    void Update() override;

    void OnTriggerStay3D(GameObject *obj);
};
