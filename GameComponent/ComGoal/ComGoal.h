/**
 * @file ComGoal.h
 * @author jupiter ()
 * @brief ComGoalクラスの宣言が記述されたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief マップのゴールに付与するゴール判定用コンポーネント
 */
class ComGoal : public ComponentBase
{
public:
    ComGoal(){};
    ~ComGoal(){};

    void OnTriggerStay3D(GameObject *obj);
};
