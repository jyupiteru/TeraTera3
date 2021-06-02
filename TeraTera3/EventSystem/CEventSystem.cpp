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
    m_collision2D = std::make_unique<CCollision2DSystem>();
    m_collision3D = std::make_unique<CCollision3DSystem>();
}

//================================================================================================
//================================================================================================

CEventSystem::~CEventSystem()
{
    m_collision2D.reset();
    m_collision3D.reset();
}

//================================================================================================
//================================================================================================

void CEventSystem::Update()
{
    m_collision2D->Update();
    m_collision3D->Update();
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
        m_collision3D->ImGuiDraw(windowid);
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
        m_collision3D->EraseCollisionObject(objectid);
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
