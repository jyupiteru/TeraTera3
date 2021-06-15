/**
 * @file CScene.h
 * @author jupiter
 * @brief CSceneが載っているヘッダ
 */

#define NOMINMAX

#include "../Setup.h"
#include "../TeraTera.h"
#include "../../GameComponent/GameComponents.h"
#include "../WindowsSystem/CDirectInput.h"

#pragma once

/**
 * @brief すべてのシーンのベースになるクラス
 */
class CScene
{
public:
    /**
	 * @brief Construct a new CScene object
	 */
    CScene(){};

    /**
	 * @brief CSceneの破棄時処理
	 */
    virtual ~CScene(){};

    /**
	 * @brief 初期処理
	 */
    virtual void Init() = 0;

    /**
	 * @brief 破棄時処理
	 */
    virtual void Uninit() = 0;

    /**
	 * @brief 毎フレームされる処理
	 */
    virtual void Update() = 0;

    /**
	 * @brief 描画処理
	 */
    virtual void Draw() = 0;
};