

#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComResultManager : public ComponentBase
{
    /**
     * @brief 時間の得点
     */
    int m_timeScore = 0;

    /**
     * @brief 今回獲得した点数を表示するテキストへのアクセス簡略用変数
     */
    Com2DText *m_comScoreText = nullptr;

    /**
     * @brief 今までプレイした中での最高得点を表示するテキストへのアクセス簡略用変数
     */
    Com2DText *m_comhighScoreText = nullptr;

    Com2DText *m_comCoinScoreText = nullptr;

    Com2DText *m_comTimeScoreText = nullptr;

public:
    ComResultManager(){};
    ~ComResultManager(){};
    void Init() override;
    void Ready() override;
    void Update() override;

private:
    /**
     * @brief 得点を計算する処理
     */
    void MathScore();
};
