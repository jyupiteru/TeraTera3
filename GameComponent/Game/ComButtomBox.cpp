/**
 * @file ComButtomBox.cpp
 * @author jupiter ()
 * @brief ComButtomBoxの実装が書かれたcpp
 */

#include "ComButtomBox.h"
#include "ComFlowManager.h"

void ComButtomBox::OnTriggerStay3D(GameObject* obj)
{
    if (obj->m_objectName == "player")
    {
        //底に敷いているこのオブジェクトに触れたので終了
        GameObject::Find("GameManager")->GetComponent<ComFlowManager>()->ChangeFlow(E_FLOW::FAILED);
    }
}