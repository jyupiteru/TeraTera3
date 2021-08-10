/**
 * @file GameComponents.h
 * @author jupiter ()
 * @brief ゲーム制作時にそのゲーム専用で制作したコンポーネントのパスを記載する場所
 */

//読み込みが複雑化しすぎてTemplate.hを読み込まなくても読み込んだことになっているバグをが出てるみたいなので注意

#pragma once

#include "ComMapMaker/ComMapMaker.h"
#include "ComMapManager/ComMapManager.h"

#include "Title/ComPlayerTitle.h"
#include "Title/ComTitleSelectorMove.h"

#include "Game/ComTimer.h"
#include "Game/ComButtomBox.h"
#include "Game/ComFlowManager.h"
#include "Game/ComStageFall.h"
#include "Game/ComPlayerMove.h"
#include "Game/ComCameraCustom.h"

#include "Goal/ComGoal.h"