/**
 * @file ComShadow.h
 * @author jupiter ()
 * @brief ComShadowの宣言が記載されたヘッダ
 */

#include <d3d11.h>
#include "../../../Core/ComponentBase.h"

class ComLight;
class ComShader;

#pragma once

/**
 * @brief 影を扱うクラス
 */
class ComShadow : public ComponentBase
{
	/**
	 * @brief GPU側に渡す情報
	 */
	struct tagConstantShadowBuffer
	{
		/**
		 * @brief 光源の位置カメラ
		 */
		DirectX::XMFLOAT4X4 ViewFromLight;

		/**
		 * @brief 光源の位置カメラに対応したプロジェクション変換行列
		 */
		DirectX::XMFLOAT4X4 ProjectionFromLight;

		/**
		 * @brief スクリーン座標をテクスチャ座標空間に変換
		 */
		DirectX::XMFLOAT4X4 ScreenToUVCoord;
	};

	/**
	 * @brief シングルトン用の変数
	 */
	static ComShadow *m_instance;

	/**
	 * @brief 影を描画したいオブジェクトの描画のみの関数を管理する変数
	 * @n オブジェクト名、描画関数
	 */
	std::unordered_map<std::string, std::function<void(void)>> m_listObjectDrawFunction;

	/**
	 * @brief 深度バッファ用の裏バッファの縦横のサイズ
	 */
	const int m_depthWidth = 4096 * 2;
	const int m_depthHeight = 4096 * 2;

	/**
	 * @brief 各種コンポーネントへの簡易アクセス用変数
	 */
	ComShader *m_comShader = nullptr;
	ComLight *m_comLight = nullptr;

	/**
	 * @brief 影の画像
	 */
	ID3D11Texture2D *m_shadowTexture = nullptr;

	/**
	 * @brief SRV
	 */
	ID3D11ShaderResourceView *m_srv = nullptr;

	ID3D11RenderTargetView *m_shadowTarget = nullptr;

	/**
	 * @brief 影用の定数バッファ
	 */
	ID3D11Buffer *m_constantShadowBuffer = nullptr;

	/**
	 * @brief テクスチャ??
	 */
	ID3D11Texture2D *m_dSTexture = nullptr;

	/**
	 * @brief DSV
	 */
	ID3D11DepthStencilView *m_dSTexDSV = nullptr;

public:
	ComShadow(){};
	~ComShadow(){};
	void Init() override;
	void Uninit() override;
	void Ready() override;
	void Update() override;
	void Draw() override;

	/**
	 * @brief 影を描画する際に使用する描画のみをする関数をセットする処理
	 * @n bindを使用すること
	 * @param _function セットしたい関数
	 */
	void SetDrawShadowFuction(std::string_view _objname, std::function<void(void)> _function);

	/**
	 * @brief 関数を削除する処理
	 * @param _objname 削除対象のオブジェクト
	 */
	void RemoveDrawFunction(std::string_view _objname);

	static ComShadow &GetInstance();

private:
	void InitDepth();

	/**
	 * @brief シャドウマップを描画する処理
	 */
	void DrawShadowMap();
};