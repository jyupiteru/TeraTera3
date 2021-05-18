/**
 * @file CSceneSample.h
 * @author jupiter
 * @brief CSceneSampleが記載されたヘッダ
 */
#include "../../Template/SceneSystem/CScene.h"

#pragma once
/**
 * @brief サンプルを載せたシーンクラス
 * @details	実験等もここで行う
 */
class CSceneSample : public CScene
{
public:
	/**
	 * @brief  CSceneSampleの生成時処理
	 */
	CSceneSample(){};

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
