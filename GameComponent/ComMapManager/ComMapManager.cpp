/**
 * @file ComMapManager.cpp
 * @author jupiter ()
 * @brief ComMapManagerの実装が記述されたcpp
 */

#include "../GameComponents.h"
#include "ComMapManager.h"

void ComMapManager::Init()
{
}

void ComMapManager::Uninit()
{
    for (auto &itr : m_listMapDate)
    {
        for (auto &itr2 : m_listMapDate[itr.first])
        {
            for (auto &itr3 : m_listMapDate[itr.first][itr2.first])
            {
                Destroy(&itr3.second);
            }
        }
    }
}

void ComMapManager::Ready()
{
}

void ComMapManager::Update()
{
}

void ComMapManager::Draw()
{
}
