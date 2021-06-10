

#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComResultManager : public ComponentBase
{
    /**
     * @brief 得点
     */
    int m_point = 0;

    Com2DText *m_comText = nullptr;

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
    void MathPoint();
};
