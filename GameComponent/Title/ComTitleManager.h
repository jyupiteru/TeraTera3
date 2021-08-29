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
public:
	/**
	 * @brief タイトルの画像のオブジェクト
	 */
	GameObject *m_titleImageObj = nullptr;

public:
	ComTitleManager(){};
	~ComTitleManager(){};
	void Update() override;
};
