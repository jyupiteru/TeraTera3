#include "ComGameManager.h"

ComGameManager *ComGameManager::m_instance = nullptr;

void ComGameManager::Init()
{
    m_instance = this;
    m_nowGameState.SetValue(E_GAMEFLOW::READY);
}

//================================================================================================
//================================================================================================

void ComGameManager::Uninit()
{
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComGameManager::Update()
{
    UpdateFlow();
}

//================================================================================================
//================================================================================================

ComGameManager &ComGameManager::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void ComGameManager::UpdateFlow()
{
    //現在のゲームのフローごとに各種処理
    switch (m_nowGameState.GetValue())
    {
    case E_GAMEFLOW::READY:
        break;

    case E_GAMEFLOW::GAME:
        break;

    case E_GAMEFLOW::END:
        break;
    }
}