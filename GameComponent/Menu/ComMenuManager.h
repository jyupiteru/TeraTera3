
/**
 * @file ComMenuManager.h
 * @author jupiter ()
 * @brief ComMenuManagerの宣言が記載されたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief メニュー関連を管理するマネージャー
 */
class ComMenuManager : public ComponentBase
{

	/**
	 * @brief 選択しているステージ番号を表示するためのコンポーネントへのポインタ
	 */
	Com2DText *m_selectStageText = nullptr;

public:
	ComMenuManager(){};
	~ComMenuManager(){};
	void Ready() override;
	void Update() override;

private:
	void UpdateStageSelect();
};
