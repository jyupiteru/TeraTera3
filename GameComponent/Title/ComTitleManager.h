/**
 * @file ComTitleManager.h
 * @author jupiter ()
 * @brief ComTitleManagerの宣言が記載されたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief タイトルの各種処理を管理するマネージェー
 */
class ComTitleManager : public ComponentBase
{

	/**
	 * @brief 選択しているステージ番号を表示するためのコンポーネントへのポインタ
	 */
	Com2DText *m_selectStageText = nullptr;

public:
	/**
	 * @brief タイトルの画像のオブジェクト
	 */
	GameObject *m_titleImageObj = nullptr;

public:
	ComTitleManager(){};
	~ComTitleManager(){};
	void Ready() override;
	void Update() override;

private:
	/**
     * @brief ステージを選択する処理
     */
	void SelectStage();
};
