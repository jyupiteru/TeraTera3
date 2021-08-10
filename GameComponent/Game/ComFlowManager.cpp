/**
 * @file ComFlowManager.cpp
 * @author jupiter ()
 * @brief ComFlowManagerの実装を記述したcpp
 */

#include "../GameComponents.h"

ComFlowManager *ComFlowManager::m_instance = nullptr;

void ComFlowManager::Init()
{
    m_instance = this;
}

//================================================================================================
//================================================================================================

void ComFlowManager::Uninit()
{
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComFlowManager::Ready()
{
    m_pComTimer = GameObject::Find("timer")->GetComponent<ComTimer>();
}

//================================================================================================
//================================================================================================

void ComFlowManager::Update()
{

    switch (m_nowFlow)
    {
    case E_FLOW::READY:
        //プレイヤーのほうで制御
        break;

    case E_FLOW::GAME:

        if (m_pComTimer->m_maxCount > 0)
        {
            m_pComTimer->UpdateTime();
        }
        else
        {
            //クリア失敗
            ChangeFlow(E_FLOW::FAILED);
        }
        break;

    case E_FLOW::GOAL:

        CSceneManager::GetInstance().LoadScene("SceneResult");

        break;

    case E_FLOW::FAILED:

        CSceneManager::GetInstance().LoadScene("SceneResult");

        break;
    default:
        break;
    }
}

//================================================================================================
//================================================================================================

void ComFlowManager::ChangeFlow(E_FLOW flow)
{
    m_nowFlow = flow;
}

//================================================================================================
//================================================================================================

ComFlowManager &ComFlowManager::GetInstance()
{
    return *m_instance;
}