
#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComTimer : public ComponentBase
{

    Com2DText *m_comText = nullptr;

public:
    float m_maxCount = 60.0f;

    ComTimer(){};
    ~ComTimer(){};

    void Init() override;

    void Update() override;
};
