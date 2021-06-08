/**
 * @file CDirectXGraphics.h
 * @brief CDirectXGraphicsが書かれたヘッダ
 */

#include <d3d11.h>

#pragma once

/**
 * @brief DirectXを使用した
 */
class CDirectXGraphics
{
private:
	/**
	 * @brief DirectXのデバイス
	 */
	ID3D11Device *m_lpDevice = nullptr;

	/**
	 * @brief DirectXコンテキスト
	 */
	ID3D11DeviceContext *m_lpImmediateContext = nullptr;

	/**
	 * @brief スワップチェイン
	 */
	IDXGISwapChain *m_lpSwapChain = nullptr;

	/**
	 * @brief レンダーターゲットビュー
	 */
	ID3D11RenderTargetView *m_lpRenderTargetView = nullptr;

	/**
	 * @brief Ｚバッファ、ステンシルバッファ
	 */
	ID3D11Texture2D *m_depthStencilBuffer = nullptr;

	/**
	 * @brief Ｚバッファ、ステンシルステート
	 */
	ID3D11DepthStencilState *m_depthStencilState = nullptr;

	/**
	 * @brief Ｚバッファ、ステンシルビュー
	 */
	ID3D11DepthStencilView *m_depthStencilView = nullptr;

	/**
	 * @brief ラスターステータス(ソリッド)
	 * @n ソリッド表示する際の各種情報を詰めた変数
	 */
	ID3D11RasterizerState *m_rasterStateSolid = nullptr;

	/**
	 * @brief  ラスターステータス(ワイヤー)
	 * @n ワイヤー表示する際の各種情報を詰めた変数
	 */
	ID3D11RasterizerState *m_rasterStateWire = nullptr;

	/**
	 * @brief ドライバの種類
	 */
	D3D_DRIVER_TYPE m_DriverType;

	/**
	 * @brief 機能レベル
	 */
	D3D_FEATURE_LEVEL m_FeatureLevel;

	/**
	 * @brief 裏のバッファの横サイズ
	 */
	int m_Width;

	/**
	 * @brief 裏のバッファの縦サイズ
	 */
	int m_Height;

	/**
	 * @brief サンプラー情報
	 */
	ID3D11SamplerState *m_samplerstate = nullptr;

	/**
	 * @brief ?? 多分アルファブレンド関係の変数 
	 * @todo  要解析アルファブレンドが効いているときの情報を集めた変数だと思う
	 */
	ID3D11BlendState *m_alphaEnableBlendingState = nullptr;

	/**
	 * @brief ?? 多分アルファブレンド関係の変数 
	 * @todo  要解析アルファブレンドが聞いていないときの情報を集めた変数だと思う
	 */
	ID3D11BlendState *m_alphaDisableBlendingState = nullptr;

public:
	CDirectXGraphics();

	/**
	 * @brief DirectX Grpaphics の初期化処理
	 * @param hWnd ウインドウハンドル値
	 * @param Width ウインドウの幅
	 * @param Height ウインドウの縦
	 * @param fullscreen フルスクリーンか?
	 * @return true 成功
	 * @return false 失敗
	 */
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool fullscreen = false);

	/**
	 * @brief 終了処理
	 */
	void Exit();

	/**
	 * @brief DirectXのデバイスを取得する処理
	 * @return ID3D11Device* 取得したいDirectX11のデバイス
	 */
	ID3D11Device *GetDXDevice() const;

	/**
	 * @brief コンテキストの取得処理
	 * @return ID3D11DeviceContext* 取得したいコンテキストのポインタ
	 */
	ID3D11DeviceContext *GetImmediateContext() const;

	/**
	 * @brief スワップチェインを取得する処理
	 * @return IDXGISwapChain* 取得したいスワップチェインのポインタ
	 */
	IDXGISwapChain *GetSwapChain() const;

	/**
	 * @brief レンダリングターゲットを取得する処理
	 * @return ID3D11RenderTargetView* 取得したいレンダリングターゲットのポインタ
	 */
	ID3D11RenderTargetView *GetRenderTargetView() const;

	/**
	 * @brief デプスステンシルビューを取得する処理
	 * @return ID3D11DepthStencilView* デプスステンシルビューのポインタ
	 */
	ID3D11DepthStencilView *GetDepthStencilView() const;

	/**
	 * @brief ウインドウの横幅の取得
	 * @return int ウインドウの横幅
	 */
	int GetWidth() const
	{
		return m_Width;
	}

	/**
	 * @brief ウインドウの縦幅の取得
	 * @return int ウインドウの横の取得
	 */
	int GetHeight() const
	{
		return m_Height;
	}

	/**
	 * @brief Zバッファの無効化処理
	 */
	void TurnOffZbuffer();

	/**
	 * @brief Zバッファの有効化処理
	 */
	void TurnOnZBuffer();

	/**
	 * @brief アルファブレンドの有効化処理
	 */
	void TurnOnAlphaBlending();

	/**
	 * @brief アルファブレンドの無効化処理
	 */
	void TurnOffAlphaBlending();

	/**
	 * @brief 表示をワイヤーフレームに変更する
	 */
	void TurnWire()
	{
		m_lpImmediateContext->RSSetState(m_rasterStateWire);
	}

	/**
	 * @brief 表示をソリッドに変更する
	 */
	void TurnSolid()
	{
		m_lpImmediateContext->RSSetState(m_rasterStateSolid);
	}
};
