
#include "../../TeraTera3/TeraTera.h"

#pragma once

enum class E_GAMEFLOW
{
    READY,
    GAME,
    END
};

class ComGameManager : public ComponentBase
{
    static ComGameManager *m_instance;

public:
    CVector<E_GAMEFLOW> m_nowGameState;

    ComGameManager(){};
    ~ComGameManager(){};

    void Init() override;

    void Uninit() override;

    void Update() override;

    [[nodiscard]] static ComGameManager &GetInstance();

private:
    void UpdateFlow();
};
