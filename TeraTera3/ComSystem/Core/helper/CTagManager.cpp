/**
 * @file CTagManager.cpp
 * @author jupiter ()
 * @brief CTagManagerの実装が書かれたcpp
 */

#include "CTagManager.h"
#include "../GameObject.h"

std::unordered_map<std::string, std::unordered_set<int>> CTagManager::m_listObjectTag;

bool CTagManager::FindObjectTag(const GameObject &obj)
{
    if (m_listObjectTag.contains(obj.m_tag))
    {
        return m_listObjectTag[obj.m_tag].contains(obj.m_objID);
    }
    return false;
}

//================================================================================================
//================================================================================================

void CTagManager::SetObjectTag(const GameObject &obj)
{
    m_listObjectTag[obj.m_tag].insert(obj.m_objID);
}

//================================================================================================
//================================================================================================

void CTagManager::EraseFromTagList(const GameObject &obj)
{
    if (m_listObjectTag.contains(obj.m_tag))
    {
        m_listObjectTag[obj.m_tag].erase(obj.m_objID);
    }
}
