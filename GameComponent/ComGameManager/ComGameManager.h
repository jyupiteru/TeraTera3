
#include "../../TeraTera3/TeraTera.h"

#pragma once
class ComGameManager : public ComponentBase
{
public:
    ComGameManager() {};
    ~ComGameManager() {};

    void Init() override;

    void Uninit() override;

    void Update() override;
};
