/**
 * @file CEventSystem.cpp
 * @author jupiter ()
 * @brief CEventSystemの実装の書かれたcpp
 */

#include "CEventSystem.h"
#include "../../ThirdParty/ImGui/imgui.h"

CEventSystem *CEventSystem::m_instance = nullptr;

CEventSystem::CEventSystem()
{
}

//================================================================================================
//================================================================================================

CEventSystem::~CEventSystem()
{
}

//================================================================================================
//================================================================================================

void CEventSystem::Update()
{
    CCollision3DSystem::GetInstance().Update();
}

//================================================================================================
//================================================================================================

void CEventSystem::Create()
{
    m_instance = new CEventSystem();

    CCollision3DSystem::Create();
}

//================================================================================================
//================================================================================================

void CEventSystem::Delete(bool _flag)
{
    if (_flag)
    {
        delete m_instance;
        m_instance = nullptr;

        CCollision3DSystem::Delete(true);
    }
}

//================================================================================================
//================================================================================================

CEventSystem &CEventSystem::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void CEventSystem::ImGuiDraw(unsigned int windowid)
{
    if (ImGui::TreeNode("CollisionSystem3D"))
    {
        CCollision3DSystem::GetInstance().ImGuiDraw(windowid);
        ImGui::TreePop();
    }
}
//================================================================================================
//================================================================================================

void CEventSystem::EraseObjectFromEvent(int objectid, E_TYPE_EVENT type)
{
    switch (type)
    {
    case E_TYPE_EVENT::COLLISION2D:
        //m_collision2D->EraseCollisionObject(objectid);
        break;

    case E_TYPE_EVENT::COLLISION3D:
        CCollision3DSystem::GetInstance().EraseCollisionObject(objectid);
        break;

    default:
        break;
    }
}

//================================================================================================
//================================================================================================

void CEventSystem::EraseObjectFromEvent(int objectid)
{
    //EraseObjectFromEvent(objectid, E_TYPE_EVENT::COLLISION2D);
    EraseObjectFromEvent(objectid, E_TYPE_EVENT::COLLISION3D);
}
