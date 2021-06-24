/**
 * @file ComWipe.h
 * @author jupiter ()
 * @brief ComWipeクラスの宣言が記載されたヘッダ
 */

#include "../../../Core/ComponentBase.h"
#include "../../Behavior/Com2DTexture/Com2DTexture.h"
#include "../../../../WindowsSystem/Dx11mathutil/Dx11mathutil.h"

#pragma once

/**
 * @brief 画像を表示し単方向のワイプをするクラス
 */
class ComWipe : public Com2DTexture
{
	ALIGN16 struct ConstantBufferWipe
	{
		/**
		 * @brief ワイプの方向
		 */
		float wipeSize;

		/**
		 * @brief パディング
		 */
		//DirectX::XMFLOAT3 pad;

		/**
		 * @brief ワイプを行う方向
		 */
		DirectX::XMFLOAT2 wipeVector;

		DirectX::XMFLOAT2 pad2;
	};

	/**
	 * @brief シェーダに情報を渡すのに必要な定数バッファ
	 */
	ID3D11Buffer *m_wideDataBuffer = nullptr;

	/**
	 * @brief 現在のワイプのサイズ
	 */
	float m_wipeSize;

	/**
	 * @brief シェーダーに渡す情報を格納する変数
	 */
	ConstantBufferWipe m_wipeData;

public:
	/**
	 * @brief ワイプをする方向
	 */
	CVector2<float> m_wipeVector;

	/**
	 * @brief ワイプの１秒当たりの進むスピード
	 */
	CVector<float> m_wipeSpeed;

public:
	ComWipe(){};
	~ComWipe(){};

	void Init() override;
	void Uninit() override;
	void Ready() override;
	void Update() override;
	void Draw() override;
};
