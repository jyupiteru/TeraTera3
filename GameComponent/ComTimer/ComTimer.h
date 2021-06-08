/**
 * @file ComTimer.h
 * @author jupiter ()
 * @brief ComTimerを宣言しているヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief 残り時間を管理するコンポーネントクラス
 */
class ComTimer : public ComponentBase
{
    /**
     * @brief 時間を表示する際に使用するテキスト
     */
    Com2DText *m_comText = nullptr;

    /**
     * @brief 残り時間
     */
    int m_timeCount = 0;

    /**
     * @brief 残り時間がintなのでその補間に使用する変数
     */
    float m_milliCount = 0.0f;

    /**
     * @brief このクラスの実体
     */
    static ComTimer *m_instance;

public:
    /**
     * @brief 何秒からカウントが始まるか
     */
    CVector<float> m_maxTimeCount;

    /**
     * @brief 現在何秒経過したか
     */
    CVector<float> m_nowCount;

public:
    ComTimer(){};
    ~ComTimer() { m_instance = nullptr; };

    void Init() override;

    void Ready() override;

    void UpdateTime();

    static ComTimer &GetInstance()
    {
        return *m_instance;
    }
};
