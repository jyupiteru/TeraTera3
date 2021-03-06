/**
 * @file Com2DTexture.h
 * @author jupiter
 * @brief Com2DTextureは乗っているヘッダ
 */
#include < memory>

#include "../Com3DTexture/Com3DTexture.h"
#include "../../../Core/ComponentBase.h"

#pragma once

/**
 * *@brief スクリーンサイズをシェーダに渡すための構造体
**/

typedef struct
{
	DirectX::XMFLOAT4 viewPortWidth;  // ビューポート幅
	DirectX::XMFLOAT4 viewPortHeight; // ビューポート高さ
} ConstantBufferViewPort;

/**
 * @brief UI（画像）表示用のクラス
 * @details 左上が(0,0)なので注意！
 */
class Com2DTexture : public Com3DTexture
{

	/**
     * @brief スクリーン情報
     */
	static ConstantBufferViewPort m_screenData; // b5

public:
	/**
	 * @brief  Component U I Textureの生成時処理
	 */
	Com2DTexture(){};

	/**
	 * @brief Component U I Textureの破棄時処理
	 */
	~Com2DTexture(){};

	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Ready() override;
	virtual void Draw() override;

	virtual void ImGuiDraw(unsigned int windowid) override;

	virtual void SetVertex() override;

	void LoadTexture(std::string texturename)
	{
		Com3DTexture::LoadTexture(texturename, E_TYPE_TEXTUREOBJ::VERTICAL);
	}
};
