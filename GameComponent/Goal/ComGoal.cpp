/**
 * @file ComGoal.cpp
 * @author jupiter ()
 * @brief ComGoalの実装が書かれたcpp
 */

#include "../GameComponents.h"

void ComGoal::OnTriggerStay3D(GameObject *obj)
{
    if (obj->m_objectName == "player")
    { //判定をして当たっていたらゴール
        GameObject::Find("GameManager")->GetComponent<ComFlowManager>()->ChangeFlow(E_FLOW::GOAL);
    }
}