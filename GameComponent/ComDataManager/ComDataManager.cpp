/**
 * @file ComDataManager.cpp
 * @author jupiter ()
 * @brief ComDataManagerクラスの実装が記述されたcpp
 */

#include "ComDataManager.h"

ComDataManager *ComDataManager::m_instance = nullptr;

void ComDataManager::Init()
{
    m_instance = this;

    m_maxTime.SetValue(60.0f);
    m_nowCount.SetValue(30.0f);
}

//================================================================================================
//================================================================================================

void ComDataManager::Uninit()
{
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

ComDataManager &ComDataManager::GetInstance()
{
    return *m_instance;
}