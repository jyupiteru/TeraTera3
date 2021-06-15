/**
 * @file Package3DBillBoard.h
 * @author jupiter 
 * @brief Package3DBillBoardが記載されているヘッダ
 */
#include "../Core/PackageBase.h"

#include "../Components/Behavior/Com3DBillBoard/Com3DBillBoard.h"

#pragma once

/**
  * @brief 画像表示（ビルボード）関係を集めたパッケージ
  */
class Package3DBillBoard : public PackageBase
{
public:
	/**
	 * @brief  Package 3 D Bill Boardの生成時処理
	 */
	Package3DBillBoard(){};

	/**
	 * @brief Package 3 D Bill Boardの破棄時処理
	 */
	~Package3DBillBoard(){};

	/**
	 * @brief 初期処理
	 */
	void Init() override
	{
		AddComponent<ComShader>();
		AddComponent<Com3DBillBoard>();
	}
};
