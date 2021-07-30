/**
 * @file CShadowManager.h
 * @author jupiter ()
 * @brief CShadowManagerクラスの宣言が記載されたヘッダ
 */

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string.h>
#include <unordered_map>
#include <functional>
#include <string_view>

#include"../../System/CVector.h"

class ComLight;

/**
 * @brief 影を表示するのに必要なシャドウマップを生成するクラス
 */
class CShadowManager final
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

	static CShadowManager *m_instance;

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
	ComLight *m_comLight = nullptr;

	/**
	 * @brief 影の画像
	 */
	ID3D11Texture2D *m_shadowTexture = nullptr;

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

private:
	CShadowManager(){};
	~CShadowManager(){};

public:
	/**
	 * @brief このマネージャーのアクティブ等を管理するフラグ
	 * @n 途中でfalseにしても影の更新処理が止まるだけなので注意
	 */
	CVector<bool> m_flagActive;

public:
	static void Create();
	static void Delete(bool _flag = false);
	static CShadowManager &GetInstance();

	/**
	 * @brief シャドウマップを生成しセットする処理
	 */
	void Update();

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

	/**
	 * @brief 影の描画対象になったいるオブジェクト名を表示する処理
	 * @param unsigned int ウインドウのID
	 */
	void ImGuiDraw(unsigned int);

private:
	void InitDepth();

	/**
	 * @brief シャドウマップを描画する処理
	 */
	void DrawShadowMap();
};
