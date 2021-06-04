
#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComTimer : public ComponentBase
{

    Com2DText *m_comText = nullptr;

    int m_timeCount = 0;

    float m_milliCount = 0.0f;
public:
    float m_maxTimeCount = 60.0f;

    ComTimer(){};
    ~ComTimer(){};

    void Init() override;

    void Update() override;
};
