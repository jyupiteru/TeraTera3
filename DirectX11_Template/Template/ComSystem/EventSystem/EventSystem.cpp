/**
 * @file EventSystem.cpp
 * @author jupiter ()
 * @brief EventSystemの実装の書かれたcpp
 */

#include "EventSystem.h"
#include "../../../Library/ImGui/imgui.h"

std::unique_ptr<CCollision2DSystem> EventSystem::m_collision2D = nullptr;
std::unique_ptr<CCollision3DSystem> EventSystem::m_collision3D = nullptr;

void EventSystem::EraseObjectFromEvent(int objectid, E_TYPE_EVENT type)
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

void EventSystem::EraseObjectFromEvent(int objectid)
{
    //EraseObjectFromEvent(objectid, E_TYPE_EVENT::COLLISION2D);
    EraseObjectFromEvent(objectid, E_TYPE_EVENT::COLLISION3D);
}

//================================================================================================
//================================================================================================

void EventSystem::ImGuiDraw(unsigned int windowid)
{
    if (ImGui::TreeNode("CollisionSystem3D"))
    {
        m_collision3D->ImGuiDraw(windowid);
        ImGui::TreePop();
    }
}