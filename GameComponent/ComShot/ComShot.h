
#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComShot : public ComponentBase
{
public:
    CVector3<float> m_shotSpeed;

    ComShot(){};

    ~ComShot(){};

    void Update();
};
