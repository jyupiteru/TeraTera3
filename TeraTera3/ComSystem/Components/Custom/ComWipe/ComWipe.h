/**
 * @file ComWipe.h
 * @author jupiter ()
 * @brief ComWipeクラスの宣言が記載されたヘッダ
 */

#include "../../../Core/ComponentBase.h"
#include "../../Behavior/Com2DTexture/Com2DTexture.h"
#include "../../../../System/Dx11mathutil/Dx11mathutil.h"

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

		//DirectX::XMFLOAT3 pad;

		/**
		 * @brief ワイプを行う方向
		 */
		DirectX::XMFLOAT2 wipeVector;

		//float pad;

		float wipeFlag;
	};

	/**
	 * @brief シェーダに情報を渡すのに必要な定数バッファ
	 */
	ID3D11Buffer *m_wideDataBuffer = nullptr;

	/**
	 * @brief シェーダーに渡す情報を格納する変数
	 */
	ConstantBufferWipe m_wipeData;

public:
	/**
	 * @brief ワイプをする方向
	 * @n xが１で右、-1で左yが+で上、-っで下
	 */
	CVector2<float> m_wipeVector;

	/**
	 * @brief ワイプの１秒当たりの進むスピード
	 */
	CVector<float> m_wipeSpeed;

	/**
	 * @brief ワイプの初期サイズ
	 * @n Speedと組み合わせる speedが+ならsizeは0で-なら最大
	 */
	CVector<float> m_wipeSize;

	/**
	 * @brief ワイプをするか、開けるか決める変数
	 * @n trueで開けて、falseで閉まる
	 */
	CVector<bool> m_wipeFlag;

public:
	ComWipe(){};
	~ComWipe(){};

	void Init() override;
	void Uninit() override;
	void Ready() override;
	void Update() override;
	void Draw() override;
};
