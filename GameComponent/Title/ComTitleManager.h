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
class ComTitleManager :public ComponentBase
{
public:
	ComTitleManager(){};
	~ComTitleManager(){};
	void Update() override;
};
