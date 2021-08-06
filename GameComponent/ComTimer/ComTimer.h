/**
 * @file ComTimer.h
 * @author jupiter ()
 * @brief ComTimerの宣言を記述したヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief タイマーとしてカウントとその表示させるコンポーネント
 */
class ComTimer : public ComponentBase
{
    /**
     * @brief 時間計測用のカウント
     */
    float m_nowCount = 0;

public:
    /**
     * @brief ゲームの制限時間
     */
    int m_maxCount = 60;

    ComTimer(){};
    ~ComTimer(){};

    void Ready() override;

    /**
     * @brief 時間の更新を行うコンポーネント
     */
    void UpdateTime();

protected:
    /**
     * @brief 表示する際に使用するテキスト表示用コンポーネント
     */
    Com2DText *m_pComText = nullptr;
};
