/**
 * @file ComGameManager.cpp
 * @author jupiter ()
 * @brief ComGameManagerの実装が書かれたcpp
 */

#include "ComGameManager.h"

#include "../ComTimer/ComTimer.h"
#include "../ComDataManager/ComDataManager.h"

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

        //残り時間が0になっていないか？
        if (ComTimer::GetInstance().m_nowCount.GetValue() < ComTimer::GetInstance().m_maxTimeCount.GetValue())
        {
            ComTimer::GetInstance().UpdateTime();
        }
        else
        { //一定時間が経過したので終了する
            m_nowGameState.SetValue(E_GAMEFLOW::END);
        }

        break;

    case E_GAMEFLOW::GAMEOVER:
        break;
    case E_GAMEFLOW::END:
        ComDataManager::GetInstance().m_maxTime.SetValue(ComTimer::GetInstance().m_maxTimeCount.GetValue());
        ComDataManager::GetInstance().m_nowCount.SetValue(ComTimer::GetInstance().m_nowCount.GetValue());

        CSceneManager::GetInstance().LoadScene("SceneResult");
        break;
    }
}