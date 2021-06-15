/**
 * @file PackageUITexture.h
 * @author jupiter 
 * @brief PackageUITextureが載っているヘッダ
 */
#include "../Core/PackageBase.h"

#include "../Components/Behavior/Com2DTexture/Com2DTexture.h"

#pragma once

/**
  * @brief UI(画像)を表示する際に使用するパッケージ
  */
class PackageUITexture : public PackageBase
{
public:
	/**
	 * @brief  Package U I Textureの生成時処理
	 */
	PackageUITexture(){};

	/**
	 * @brief Package U I Textureの破棄時処理
	 */
	~PackageUITexture(){};

	/**
	 * @brief 初期処理
	 */
	void Init() override
	{
		AddComponent<ComShader>();
		AddComponent<Com2DTexture>();
	}
};
