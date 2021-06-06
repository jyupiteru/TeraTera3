
#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComTimer : public ComponentBase
{

    Com2DText *m_comText = nullptr;

    int m_timeCount = 0;

    float m_milliCount = 0.0f;

    static ComTimer *m_instance;

public:
    CVector<float> m_maxTimeCount;

    CVector<float> m_nowCount;

    ComTimer(){};
    ~ComTimer() { m_instance = nullptr; };

    void Init() override;

    void Ready() override;

    void Update() override;

    static ComTimer &GetInstance()
    {
        return *m_instance;
    }
};
