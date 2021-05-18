/**
 * @file ControlObjPriority.cpp
 * @author jupiter ()
 * @brief ControlObjPriorityの実装部分が書かれたcpp
 */

#include "ControlObjPriority.h"
#include "../GameObject.h"
#include "../../Components/Behavior/ComTransform/ComTransform.h"

std::multimap<int, int> ControlObjectPriority::m_listObjectForUpdate;

std::map<int, std::map<int, std::map<float, std::multimap<int, int>>>> ControlObjectPriority::m_listObjectForDraw;

GameObject *ControlObjectPriority::m_pCameraObject = nullptr;

void ControlObjectPriority::SetObjectUpdateOrder(const GameObject &obj)
{
    auto id = obj.m_objID;
    auto priority = obj.m_objectUpdatePriority.GetValue();

    int min = -30, max = 200;

    //min,maxの範囲に収める
    priority = std::clamp(priority, min, max);

    m_listObjectForUpdate.insert(std::make_pair(priority, id));
}

//================================================================================================
//================================================================================================

void ControlObjectPriority::SetObjectDrawingOrder(const GameObject &obj, const float distance)
{
    auto [r, b, d, a] = obj.m_transform->m_color.GetValue();

    auto objid = obj.m_objID;

    auto type = obj.m_typeObject;

    auto layer = obj.m_drawLayer.GetValue();

    auto priority = obj.m_objectDrawPriority.GetValue();

    int objtype = 0;

    //各最適なリストに格納
    if (a == 1.0f)
    {
        objtype = 0;
    }
    else
    {
        objtype = 1;
    }

    //同じ距離の位置に同じ描画順のものが存在しているので追加順に描画

    m_listObjectForDraw[layer][objtype][distance].insert(std::make_pair(priority, objid));
}

//================================================================================================
//================================================================================================

void ControlObjectPriority::ResetUpdateList()
{
    m_listObjectForUpdate.clear();
}

//================================================================================================
//================================================================================================

void ControlObjectPriority::ResetDrawList()
{
    for (auto itr : m_listObjectForDraw)
    {
        for (auto itr2 : itr.second)
        {
            for (auto itr3 : itr2.second)
            {
                itr3.second.clear();
            }
        }
    }
    m_listObjectForDraw.clear();
}