/**
 * @file ComWipe.h
 * @author jupiter ()
 * @brief ComWipeクラスの宣言が記載されたヘッダ
 */

#include "../../../Core/ComponentBase.h"

#pragma once

class ComWipe : public ComponentBase
{

public:
	ComWipe(){};
	~ComWipe(){};

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	
	
};
