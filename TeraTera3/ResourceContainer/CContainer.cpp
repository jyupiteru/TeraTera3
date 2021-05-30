/**
 * @file CContainer.cpp
 * @author jupiter ()
 * @brief CContainerクラスの実装を記述したcpp
 */

#include "CContainer.h"

CContainer *CContainer::m_instance = nullptr;

void CContainer::Create()
{
     m_instance = new CContainer();
}

//================================================================================================
//================================================================================================

void CContainer::Delete(bool _flag)
{
     if (_flag)
     {
          delete m_instance;
          m_instance = nullptr;
     }
}

//================================================================================================
//================================================================================================

CContainer & CContainer::GetInstance()
{
     return *m_instance;
}