
#include "../../TeraTera3/TeraTera.h"

#pragma once

enum class E_PLAYERFLOW
{
    READY,
    GAME
};

class ComPlayer : public ComponentBase
{

    E_PLAYERFLOW m_nowstate = E_PLAYERFLOW::READY;

    float m_playerSpeed = 10.0f;

public:
    ComPlayer(){};
    ~ComPlayer(){};

    void Init() override;

    void Uninit() override;

    void Ready() override;

    void Update() override;

    void OnTriggerStay3D(GameObject *obj);

private:
    void PlayerMove();
};
