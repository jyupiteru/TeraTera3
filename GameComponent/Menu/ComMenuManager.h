
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

	/**
	 * @brief セレクトのオブジェクト
	 */
	GameObject *m_selecterObject = nullptr;

public:
	ComMenuManager(){};
	~ComMenuManager(){};
	void Init() override;
	void Ready() override;
	void Update() override;

private:
	/**
	 * @brief ステージ選択周りのアップデート管理
	 */
	void UpdateStageSelect();

	/**
	 * @brief カスタム、ゲームスタート周りの管理
	 */
	void UpdateSelect();
};
