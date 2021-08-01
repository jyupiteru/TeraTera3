/**
 * @file Package3DModelObject.h
 * @author jupiter
 * @brief Package4DModelを記述したヘッダ
 */
#include "../Core/PackageBase.h"

#include "../Components/Behavior/Com3DModelAssimp/Com3DModelAssimp.h"

#pragma once

/**
 * @brief Assimpを使用してモデルを表示するために必要なコンポーネントを集めたもの
 */
class Package3DModelObjectAssimp : public PackageBase
{
public:
	/**
	 * @brief  Package 3 D Model Objectの生成時処理
	 */
	Package3DModelObjectAssimp(){};

	/**
	 * @brief Package 3 D Model Objectの破棄時処理
	 */
	~Package3DModelObjectAssimp(){};

	/**
	 * @brief 初期処理
	 */
	void Init() override
	{
		AddComponent<ComShader>();
		AddComponent<Com3DModelAssimp>();
	}
};
