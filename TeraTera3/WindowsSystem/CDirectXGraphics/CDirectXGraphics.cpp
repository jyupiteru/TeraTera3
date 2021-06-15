﻿/**
 * @file CDirectXGraphics.cpp
 * @brief CDirectXGraphicsの実装が書かれたcpp
 */

#include "CDirectXGraphics.h"

CDirectXGraphics *CDirectXGraphics::m_instance = nullptr;

CDirectXGraphics::CDirectXGraphics()
{
	m_lpDevice = NULL;
	m_lpImmediateContext = NULL;
	m_Height = 0;
	m_Width = 0;
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::Create()
{
	m_instance = new CDirectXGraphics();
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::Delete(bool _flag)
{
	if (_flag)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

//================================================================================================
//================================================================================================

CDirectXGraphics &CDirectXGraphics::GetInstance()
{
	return *m_instance;
}

//================================================================================================
//================================================================================================

bool CDirectXGraphics::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool fullscreen)
{
	HRESULT hr = S_OK;
	IDXGIFactory *factory;		// factory
	IDXGIAdapter *adapter;		// videocard
	IDXGIOutput *adapterOutput; // monitor
	unsigned int numModes;
	unsigned int numerator = 60;  // 分子
	unsigned int denominator = 1; // 分母
	DXGI_MODE_DESC *displayModeList;

	m_Width = Width;
	m_Height = Height;

	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP, // Windows Advanced Rasterizer
			D3D_DRIVER_TYPE_REFERENCE,
		};
	unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

	unsigned int createDeviceFlags = 0;
	//   createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1, // DirectX11.1対応GPUレベル
			D3D_FEATURE_LEVEL_11_0, // DirectX11対応GPUレベル
			D3D_FEATURE_LEVEL_10_1, // DirectX10.1対応GPUレベル
			D3D_FEATURE_LEVEL_10_0, // DirectX10対応GPUレベル
			D3D_FEATURE_LEVEL_9_3,	// DirectX9.3対応GPUレベル
			D3D_FEATURE_LEVEL_9_2,	// DirectX9.2対応GPUレベル
			D3D_FEATURE_LEVEL_9_1	// Direct9.1対応GPUレベル
		};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Create a DirectX graphics interface factory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
	if (FAILED(hr))
	{
		return false;
	}

	// use the factory to create an adpter for the primary graphics interface(video card)
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
	{
		return false;
	}

	// enumerrate primary adapter output(monitor)
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
	{
		return false;
	}

	// get the number of modes that fit the DXGI_FORMAT_R8G8B8_UNORM display format forthe adapter output(monitor)
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// create alist to hold all possible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// now fill the display mode list structures
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
	{
		return false;
	}

	// now go through all the display modes and find the one that matches the screen width and height
	// when a match is found store the numerator and denominator of the refresh rate for that monitor
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == Width)
		{
			if (displayModeList[i].Height == Height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	// スワップチェインの設定
	// スワップチェインとは、ウインドウへの表示ダブルバッファを管理する
	// クラス　マルチサンプリング、リフレッシュレートが設定できる
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						 // ゼロクリア
	sd.BufferCount = 1;									 // バックバッファの数
	sd.BufferDesc.Width = Width;						 // バックバッファの幅
	sd.BufferDesc.Height = Height;						 // バックバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	 // バックバッファフォーマット(R,G,B 範囲０．０から１．０)
	sd.BufferDesc.RefreshRate.Numerator = numerator;	 // リフレッシュレート（分母）
	sd.BufferDesc.RefreshRate.Denominator = denominator; // リフレッシュレート（分子）
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バックバッファの使用方法
	sd.OutputWindow = hWnd;							  // 関連付けるウインドウ
	sd.SampleDesc.Count = 1;						  // マルチサンプルの数
	sd.SampleDesc.Quality = 0;						  // マルチサンプルのクオリティ
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (fullscreen)
	{
		sd.Windowed = false; // ウインドウモード
	}
	else
	{
		sd.Windowed = TRUE; // ウインドウモード
	}
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // モードの自動切り替え

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL,				   // ディスプレイデバイスのアダプタ（ＮＵＬＬの場合最初に見つかったアダプタ）
										   m_DriverType,		   // デバイスドライバのタイプ
										   NULL,				   // ソフトウェアラスタライザを使用する場合に指定する
										   createDeviceFlags,	   // デバイスフラグ
										   featureLevels,		   // 機能レベル
										   numFeatureLevels,	   // 機能レベル数
										   D3D11_SDK_VERSION,	   //
										   &sd,					   // スワップチェインの設定
										   &m_lpSwapChain,		   // IDXGIDwapChainインタフェース
										   &m_lpDevice,			   // ID3D11Deviceインタフェース
										   &m_FeatureLevel,		   // サポートされている機能レベル
										   &m_lpImmediateContext); // デバイスコンテキスト
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
	{
		return false;
	}

	// レンダリングターゲットを作成
	// バックバッファのポインタを取得
	ID3D11Texture2D *pBackBuffer = NULL;
	hr = m_lpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);
	if (FAILED(hr))
		return false;

	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	hr = m_lpDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_lpRenderTargetView);
	pBackBuffer->Release();
	pBackBuffer = 0;
	if (FAILED(hr))
		return false;

	// Zバッファ
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = m_lpDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// ステンシルステート作成

	// ステンシル設定構造体初期化
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// set up the description of the stencl state
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// stencil operation if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operation if pixel is　back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create the depth stencil state
	hr = m_lpDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	// デバイスコンテキストへセット
	m_lpImmediateContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// depthstencilview 初期化
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// create the depth stencil view
	hr = m_lpDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	// bind the render target view and depth stencil buffer to the output render pipeline
	m_lpImmediateContext->OMSetRenderTargets(1, &m_lpRenderTargetView, m_depthStencilView);

	// setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	//ここでカリング変更可能
	//rasterDesc.CullMode = D3D11_CULL_NONE;//両面カリング
	rasterDesc.CullMode = D3D11_CULL_BACK; //法線の向いてる方向の逆カリング
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	//rasterDesc.FillMode = D3D11_FILL_WIREFRAME; // ワイヤフレームにしたいとき
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create the rasterrizer state from the description we just filled out
	hr = m_lpDevice->CreateRasterizerState(&rasterDesc, &m_rasterStateSolid);
	if (FAILED(hr))
	{
		return false;
	}

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	// create the rasterrizer state from the description we just filled out
	hr = m_lpDevice->CreateRasterizerState(&rasterDesc, &m_rasterStateWire);
	if (FAILED(hr))
	{
		return false;
	}

	//  set the rasterizer state
	m_lpImmediateContext->RSSetState(m_rasterStateSolid);

	// ビューポートを設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Width;
	vp.Height = (FLOAT)Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_lpImmediateContext->RSSetViewports(1, &vp);

	m_Height = Height;
	m_Width = Width;

	//ブレンドステート初期化
	D3D11_BLEND_DESC blendStateDescription;

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	//ブレンドステート設定（アルファブレンド可）
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//ブレンドステート作成
	hr = m_lpDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(hr))
	{
		return false;
	}

	//ブレンドステート設定（アルファブレンド不可）
	blendStateDescription.RenderTarget[0].BlendEnable = false;

	//ブレンドステート作成
	hr = m_lpDevice->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(hr))
	{
		return false;
	}

	m_Height = Height;
	m_Width = Width;

	D3D11_SAMPLER_DESC smpDesc;
	ZeroMemory(&smpDesc, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	/*smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;*/
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// サンプラーステート生成
	hr = m_lpDevice->CreateSamplerState(&smpDesc, &m_samplerstate);
	if (FAILED(hr))
	{
		return false;
	}

	// サンプラーステートを転送
	m_lpImmediateContext->PSSetSamplers(0, 1, &m_samplerstate);

	return (true);
}

//================================================================================================
//================================================================================================
void CDirectXGraphics::Exit()
{
	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = nullptr;
	}

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = nullptr;
	}

	if (m_lpImmediateContext)
	{
		m_lpImmediateContext->ClearState();
	}
	if (m_lpSwapChain)
	{
		m_lpSwapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterStateSolid)
	{
		m_rasterStateSolid->Release();
		m_rasterStateSolid = 0;
	}

	if (m_rasterStateWire)
	{
		m_rasterStateWire->Release();
		m_rasterStateWire = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_lpRenderTargetView)
	{
		m_lpRenderTargetView->Release();
		m_lpRenderTargetView = 0;
	}

	if (m_lpImmediateContext)
	{
		m_lpImmediateContext->Release();
		m_lpImmediateContext = 0;
	}

	if (m_lpDevice)
	{
		m_lpDevice->Release();
		m_lpDevice = 0;
	}

	if (m_lpSwapChain)
	{
		m_lpSwapChain->Release();
		m_lpSwapChain = 0;
	}
	return;
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::BeforeDraw(float _clearcolor[])
{
	m_lpImmediateContext->ClearRenderTargetView(m_lpRenderTargetView, _clearcolor);
	// Zバッファ(深度バッファ)クリア
	m_lpImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::AfterDraw()
{
	// レンダリングされたイメージを表示。
	m_lpSwapChain->Present(0, 0);
}

//================================================================================================
//================================================================================================

ID3D11Device *CDirectXGraphics::GetDXDevice() const
{
	return m_lpDevice;
}

//================================================================================================
//================================================================================================

ID3D11DeviceContext *CDirectXGraphics::GetImmediateContext() const
{
	return m_lpImmediateContext;
}

//================================================================================================
//================================================================================================

IDXGISwapChain *CDirectXGraphics::GetSwapChain() const
{
	return m_lpSwapChain;
}

//================================================================================================
//================================================================================================

ID3D11RenderTargetView *CDirectXGraphics::GetRenderTargetView() const
{
	return m_lpRenderTargetView;
}

//================================================================================================
//================================================================================================

ID3D11DepthStencilView *CDirectXGraphics::GetDepthStencilView() const
{
	return m_depthStencilView;
}

//================================================================================================
//================================================================================================

int CDirectXGraphics::GetWidth() const
{
	return m_Width;
}

//================================================================================================
//================================================================================================

int CDirectXGraphics::GetHeight() const
{
	return m_Height;
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::TurnOffZbuffer()
{
	ID3D11RenderTargetView *rtvtable[1];

	rtvtable[0] = m_lpRenderTargetView;

	m_lpImmediateContext->OMSetRenderTargets(
		1,		  // ターゲット
		rtvtable, // ビューテーブル
		nullptr	  // 深度バッファなし
	);
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::TurnOnZBuffer()
{
	ID3D11RenderTargetView *rtvtable[1];

	rtvtable[0] = m_lpRenderTargetView;

	m_lpImmediateContext->OMSetRenderTargets(
		1,				   // ターゲット
		rtvtable,		   // ビューテーブル
		m_depthStencilView // 深度バッファなし
	);
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::TurnOnAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	//アルファブレンドをONにする
	m_lpImmediateContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
	return;
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::TurnOffAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	//アルファブレンドをOFFにする
	m_lpImmediateContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
	return;
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::TurnWire()
{
	m_lpImmediateContext->RSSetState(m_rasterStateWire);
}

//================================================================================================
//================================================================================================

void CDirectXGraphics::TurnSolid()
{
	m_lpImmediateContext->RSSetState(m_rasterStateSolid);
}