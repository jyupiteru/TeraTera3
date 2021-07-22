/**
 * @file CSceneSample3.h
 * @author jupiter ()
 * @brief CSceneSample3の宣言が記入されたヘッダ
 */
#include "../../TeraTera3/SceneSystem/CScene.h"


#pragma once

/**
 * @brief テスト用のシーンクラス２
 */
class CSceneSample3 : public CScene
{
public:
    /**
	 * @brief  CSceneSample3の生成時処理
	 */
    CSceneSample3(){};

    /**
	 * @brief 初期処理
	 */
    void Init() override;

    /**
	 * @brief 破棄時処理
	 */
    void Uninit() override;

    /**
	 * @brief 毎フレームする処理
	 */
    void Update() override;

    /**
	 * @brief 描画処理
	 */
    void Draw() override;
};
