/**
 * @file CSceneSample2.h
 * @author jupiter ()
 * @brief CSceneSample2の宣言が記入されたヘッダ
 */
#include "../../TeraTera3/Managers/SceneSystem/CScene.h"


#pragma once

/**
 * @brief テスト用のシーンクラス２
 */
class CSceneSample2 : public CScene
{
public:
    /**
	 * @brief  CSceneSample2の生成時処理
	 */
    CSceneSample2(){};

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
