/**
 * @file ComShadow.cpp
 * @author jupiter ()
 * @brief ComShadowクラスの実装が記載されたcpp
 */

#include "ComShadow.h"
#include "../../../../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"

ComShadow *ComShadow::m_instance = nullptr;

using namespace DirectX;

void ComShadow::Init()
{
	if (m_instance == nullptr)
	{
		m_instance = this;

		//描画は一番初めにしないといけないのでこれで
		m_gameObject->m_drawLayer.SetValue(-1000);
	}
	else
	{ //複数読み込むことはないため
		this->m_gameObject->RemoveComponent<ComShadow>();
	}
}

//================================================================================================
//================================================================================================

void ComShadow::Uninit()
{
	m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComShadow::Ready()
{
	m_comShader = this->m_gameObject->GetComponent<ComShader>();
	if (m_comShader == nullptr)
	{
		m_comShader = m_gameObject->AddComponent<ComShader>();
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	unsigned int numelements = ARRAYSIZE(layout);

	m_comShader->LoadVertexShader("VSShadow.fx", layout, numelements);
	m_comShader->LoadPixelShader("PSShadow.fx");

	//ライトコンポーネントを取得
	m_comLight = GameObject::Find("Light")->GetComponent<ComLight>();
}

//================================================================================================
//================================================================================================

void ComShadow::Update()
{
}

//================================================================================================
//================================================================================================

void ComShadow::Draw()
{
	DrawShadowMap();
}

//================================================================================================
//================================================================================================

void ComShadow::SetDrawShadowFuction(std::string_view _objname, std::function<void(void)> _function)
{
	//まだセットしていないか？
	if (!m_listObjectDrawFunction.contains(_objname.data()))
	{
		m_listObjectDrawFunction[_objname.data()] = _function;
	}
}

//================================================================================================
//================================================================================================

void ComShadow::RemoveDrawFunction(std::string_view _objname)
{
	if (m_listObjectDrawFunction.contains(_objname.data()))
	{
		m_listObjectDrawFunction.erase(_objname.data());
	}
}

//================================================================================================
//================================================================================================

void ComShadow::InitDepth()
{

	ID3D11Device *device;
	device = CDirectXGraphics::GetInstance().GetDXDevice();

	//深度マップテクスチャーを作成
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_depthWidth;
	desc.Height = m_depthHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.CPUAccessFlags = 0; // CPUからのアクセスなし
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.MiscFlags = 0; // その他設定なし
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT; // デフォルトの使用法

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット

	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;				// 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;					// すべてのミップマップ レベル

	// 2Dテクスチャを生成
	HRESULT hr = device->CreateTexture2D(
		&desc,			 // 作成する2Dテクスチャの設定
		nullptr,		 //
		&g_DepthMapTex); // 作成したテクスチャを受け取る変数
	if (FAILED(hr))
		MessageBox(nullptr, "CreateTexture error", "Error", MB_OK);

	// シェーダ リソース ビューの生成
	hr = device->CreateShaderResourceView(
		g_DepthMapTex.Get(), // アクセスするテクスチャ リソース
		&srDesc,			 // シェーダ リソース ビューの設定
		&g_DepthMapSRV);	 // ＳＲＶ受け取る変数
	if (FAILED(hr))
		MessageBox(nullptr, "SRV error", "Error", MB_OK);

	// レンダーターゲットビュー生成
	hr = device->CreateRenderTargetView(
		g_DepthMapTex.Get(), // ２Ｄテクスチャ
		nullptr,			 // ＲＴＶの設定（今回はなし）
		&g_DepthMapRTV);	 // ＲＴＶを受け取る変数
	if (FAILED(hr))
		MessageBox(nullptr, "RTV error", "Error", MB_OK);

	//デプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = m_depthWidth;
	descDepth.Height = m_depthHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;

	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = device->CreateTexture2D(&descDepth, nullptr, &g_DSTex);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateTexture2D error", "error", MB_OK);
		return;
	}

	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	hr = device->CreateDepthStencilView(g_DSTex.Get(), nullptr, &g_DSTexDSV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateDepthStencilView error", "error", MB_OK);
		return;
	}

	// 定数バッファ生成
	sts = CreateConstantBufferWrite(
		device,
		sizeof(ConstantBufferShadowmap),
		&g_ConstantBufferShadowmap); // シャドウマップ用定数バッファ
}

//================================================================================================
//================================================================================================

void ComShadow::DrawShadowMap()
{
	// ターゲットバッファクリア
	float ClearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// デバイスコンテキスト取得
	ID3D11DeviceContext *devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();

	// レンダリングターゲットビュー、デプスステンシルビューを設定
	ID3D11RenderTargetView *rtv[] = {g_DepthMapRTV.Get()};
	devcontext->OMSetRenderTargets(1, rtv, g_DSTexDSV.Get());

	// ビューポートを設定
	D3D11_VIEWPORT vp;
	vp.Width = m_depthWidth;
	vp.Height = m_depthHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	devcontext->RSSetViewports(1, &vp);

	// ターゲットバッファクリア
	devcontext->ClearRenderTargetView(g_DepthMapRTV.Get(), ClearColor);
	// Zバッファクリア
	devcontext->ClearDepthStencilView(g_DSTexDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//各シェーダーのセット
	m_comShader->SetVertexShader();
	m_comShader->SetPixelShader();

	//車道マップではライト目線のカメラを用意する必要があるのでライト座標取得でやる
	DirectX::XMFLOAT3 lightpos;

	//ライトの座標(ぢレクションライトの方向)を取得 & 正規化
	auto [light_x, light_y, light_z] = m_comLight->m_lightDirection.GetValue();
	lightpos = DirectX::XMFLOAT3(light_x, light_y, light_z);
	DirectX::XMVECTOR v;
	v = DirectX::XMLoadFloat3(&lightpos);
	v = DirectX::XMVector3Normalize(v);
	DirectX::XMStoreFloat3(&lightpos, v);

	lightpos.x *= 100;
	lightpos.z *= 100;
	lightpos.y *= 100;

	DirectX::XMFLOAT4 l;
	l.x = lightpos.x;
	l.y = lightpos.y;
	l.z = lightpos.z;
	l.w = 1.0f;
	light.SetLightPos(l);
	light.Update();

	//車道マップを生成するために光源からみたカメラを生成する
	ALIGN16 DirectX::XMVECTOR eye = DirectX::XMVectorSet(lightpos.x, lightpos.y, lightpos.z, 0.0f);
	ALIGN16 DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ALIGN16 DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	ALIGN16 DirectX::XMMATRIX camera;
	camera = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, at, up));

	// ビュー変換行列をセット（光源位置からのカメラ）
	ConstantBufferShadowmap cb;
	XMStoreFloat4x4(&cb.ViewFromLight, camera);

	// 光源カメラ用のプロジェクション変換行列
	float nearclip = 10.0f;
	float farclip = 300.0f; // ファークリップの値がおかしくてバグった
	float Aspect = 1.0f;
	float Fov = DirectX::XM_PI / 3;

	ALIGN16 DirectX::XMMATRIX ProjectionFromLight;

	ProjectionFromLight = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(Fov, Aspect, nearclip, farclip));
	DirectX::XMStoreFloat4x4(&cb.ProjectionFromLight, ProjectionFromLight); // プロジェクション変換行列をセット

	// スクリーン座標をＵＶ座標に変換する行列
	DirectX::XMStoreFloat4x4(&cb.ScreenToUVCoord, DirectX::XMMatrixTranspose(g_uvmatrix));

	// 定数バッファに反映させる
	D3D11_MAPPED_SUBRESOURCE pData;

	HRESULT hr = devcontext->Map(g_ConstantBufferShadowmap.Get(),
								 0,
								 D3D11_MAP_WRITE_DISCARD,
								 0,
								 &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, (void *)(&cb), sizeof(ConstantBufferShadowmap));
		devcontext->Unmap(g_ConstantBufferShadowmap.Get(), 0);
	}

	// 定数バッファを8番スロットにセット
	devcontext->PSSetConstantBuffers(8, 1, g_ConstantBufferShadowmap.GetAddressOf());
	devcontext->VSSetConstantBuffers(8, 1, g_ConstantBufferShadowmap.GetAddressOf());

	//影を描画したいオブジェクトの描画処理を行いテクスチャを生成
	for (auto itr : m_listObjectDrawFunction)
	{
		itr.second();
	}
}