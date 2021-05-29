///**
// * @file ComCollider2D.cpp
// * @author jupiter ()
// * @brief ComCollider2Dの実装が書かれたcpp
// */
//#include "ComCollider2D.h"
//#include "../../DefaultComponents.h"
//#include "../../../EventSystem/EventSystem.h"
//
//void ComCollider2D::Uninit()
//{
//    EventSystem::EraseObjectFromEvent(this->m_gameObject, E_TYPE_EVENT::COLLISION2D);
//}
//
//void ComCollider2D::Ready()
//{
//    if (this->m_gameObject->GetComponent<ComUITexture>())
//    {
//        //このオブジェクトを当たり判定リストに登録
//        EventSystem::SetObjectToEvent(this->m_gameObject, E_TYPE_EVENT::COLLISION2D);
//
//        if (!this->m_gameObject->GetComponent<ComRigidBody2D>())
//        {
//            m_isTrigger.SetValue(false);
//        }
//    }
//}