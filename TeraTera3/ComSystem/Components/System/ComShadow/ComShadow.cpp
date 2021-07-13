/**
 * @file ComShadow.cpp
 * @author jupiter ()
 * @brief ComShadowクラスの実装が記載されたcpp
 */

#include "ComShadow.h"
#include "../../../../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"

ComShadow *ComShadow::m_instance = nullptr;

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
	unsigned int numElements = ARRAYSIZE(layout);

	m_comShader->LoadVertexShader("VSShadow.fx", layout, numElements);
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
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
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
	DirectX::XMVector3 lightpos;

	//ライトの座標を取得
	auto [light_x, light_y, light_z] = m_comLight->m_lightDirection.GetValue();
	lightpos = DirectX::XMFLOAT3(light_x, light_y, light_z);
	DirectX::XMVector3Normalize(lightpos);


	LightPos.x = 100 *;
	LightPos.z = 100 *;
	LightPos.y = 100 *;

	DirectX::XMFLOAT4 l;
	l.x = LightPos.x;
	l.y = LightPos.y;
	l.z = LightPos.z;
	l.w = 1.0f;
	light.SetLightPos(l);
	light.Update();

	ALIGN16 DirectX::XMVECTOR Eye = DirectX::XMVectorSet(LightPos.x, LightPos.y, LightPos.z, 0.0f);
	ALIGN16 DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ALIGN16 DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	ALIGN16 DirectX::XMMATRIX camera;
	camera = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(Eye, At, Up));

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