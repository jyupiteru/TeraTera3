/**
 * @file GameComponents.h
 * @author jupiter ()
 * @brief ゲーム制作時にそのゲーム専用で制作したコンポーネントのパスを記載する場所
 */

//読み込みが複雑化しすぎてTemplate.hを読み込まなくても読み込んだことになっているバグをが出てるみたいなので注意

#pragma once

#include "ComDataManager/ComDataManager.h"

#include "Title/ComPlayerTitle.h"
#include "Title/ComTitleManager.h"

#include "Menu/ComMenuSelectorMove.h"
#include "Menu/ComMenuManager.h"

#include "Game/ComTimer.h"
#include "Game/ComButtomBox.h"
#include "Game/ComFlowManager.h"
#include "Game/ComStageManager.h"
#include "Game/ComStageFall.h"
#include "Game/ComPlayerMove.h"
#include "Game/ComCameraCustom.h"
#include "Game/ComGoal.h"