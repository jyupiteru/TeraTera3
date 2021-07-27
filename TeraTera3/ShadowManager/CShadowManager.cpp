
/**
 * @file CShadowManager.cpp
 * @author jupiter ()
 * @brief CShadowManagerクラスの実装が記載されたcpp
 */

#include "CShadowManager.h"
#include "../ShaderManager/CShaderManager.h"
#include "../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"
#include "../ComSystem/Core/Cores.h"
#include "../ComSystem/Components/System/ComLight/ComLight.h"
#include "../WindowsSystem/Shader/Shader.h"
#include "../../ThirdParty/ImGui/imgui.h"
#include"../System/CTextureManager/CTextureManager.h"

CShadowManager *CShadowManager::m_instance = nullptr;

using namespace DirectX;

void CShadowManager::Create()
{
	if (m_instance == nullptr)
	{
		m_instance = new CShadowManager();

		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
		unsigned int numelements = ARRAYSIZE(layout);

		CShaderManager::GetInstance().LoadVertexShader("VSShadowMap.fx", layout, numelements);
		CShaderManager::GetInstance().LoadPixelShader("PSShadowMap.fx");

		m_instance->InitDepth();
	}
}

//================================================================================================
//================================================================================================

void CShadowManager::Delete(bool _flag)
{
	if (_flag)
	{
		m_instance->m_listObjectDrawFunction.clear();
		delete m_instance;
		m_instance = nullptr;
	}
}

//================================================================================================
//================================================================================================

CShadowManager &CShadowManager::GetInstance()
{
	return *m_instance;
}

//================================================================================================
//================================================================================================

void CShadowManager::SetDrawShadowFuction(std::string_view _objname, std::function<void(void)> _function)
{
	//まだセットしていないか？
	if (!m_listObjectDrawFunction.contains(_objname.data()))
	{
		m_listObjectDrawFunction[_objname.data()] = _function;
	}
}

//================================================================================================
//================================================================================================

void CShadowManager::RemoveDrawFunction(std::string_view _objname)
{
	if (m_listObjectDrawFunction.contains(_objname.data()))
	{
		m_listObjectDrawFunction.erase(_objname.data());
	}
}

//================================================================================================
//================================================================================================

void CShadowManager::CreateShadowMap()
{
	if (m_comLight == nullptr)
	{
		m_comLight = GameObject::Find("Light")->GetComponent<ComLight>();
	}

	DrawShadowMap();

	// デバイスコンテキスト取得
	ID3D11DeviceContext *devcontext;
	devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();

	// レンダリングターゲットビュー、デプスステンシルビューを設定
	ID3D11RenderTargetView *rtv[] = {CDirectXGraphics::GetInstance().GetRenderTargetView()};
	devcontext->OMSetRenderTargets(1, rtv, CDirectXGraphics::GetInstance().GetDepthStencilView());

	// ビューポートを設定
	D3D11_VIEWPORT vp;

	vp.Width = static_cast<float>(CDirectXGraphics::GetInstance().GetWidth());
	vp.Height = static_cast<float>(CDirectXGraphics::GetInstance().GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	devcontext->RSSetViewports(1, &vp);

	// 定数バッファを8番スロットにセット
	devcontext->PSSetConstantBuffers(8, 1, &m_constantShadowBuffer);
	devcontext->VSSetConstantBuffers(8, 1, &m_constantShadowBuffer);

	tagTextureData* texture =  CTextureManager::GetInstance().GetTextureData("ShadowTexture");

	// depthmapをセット
	devcontext->PSSetShaderResources(1, 1, &texture->srv);
}

//================================================================================================
//================================================================================================

void CShadowManager::ImGuiDraw(unsigned int)
{
	//影を描画しているオブジェクトを表示する
	if (ImGui::TreeNode("Draw Shadow Object"))
	{
		for (auto &itr : m_listObjectDrawFunction)
		{
			ImGui::BulletText(itr.first.c_str());
		}
		ImGui::TreePop();
	}
}

//================================================================================================
//================================================================================================

void CShadowManager::InitDepth()
{

	ID3D11Device *device;
	device = CDirectXGraphics::GetInstance().GetDXDevice();

	//深度マップテクスチャーを作成
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
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
	D3D11_SHADER_RESOURCE_VIEW_DESC srdesc;
	srdesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット

	srdesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srdesc.Texture2D.MostDetailedMip = 0;				// 最初のミップマップ レベル
	srdesc.Texture2D.MipLevels = -1;					// すべてのミップマップ レベル

	// 2Dテクスチャを生成
	HRESULT hr = device->CreateTexture2D(
		&desc,			   // 作成する2Dテクスチャの設定
		nullptr,		   //
		&m_shadowTexture); // 作成したテクスチャを受け取る変数
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateTexture error", "Error", MB_OK);
	}

	ID3D11ShaderResourceView* srv;

	// シェーダ リソース ビューの生成
	hr = device->CreateShaderResourceView(
		m_shadowTexture, // アクセスするテクスチャ リソース
		&srdesc,		 // シェーダ リソース ビューの設定
		&srv);		 // ＳＲＶ受け取る変数
	if (FAILED(hr))
	{
		MessageBox(nullptr, "SRV error", "Error", MB_OK);
	}

	// レンダーターゲットビュー生成
	hr = device->CreateRenderTargetView(
		m_shadowTexture,  // ２Ｄテクスチャ
		nullptr,		  // ＲＴＶの設定
		&m_shadowTarget); // ＲＴＶを受け取る変数
	if (FAILED(hr))
	{
		MessageBox(nullptr, "RTV error", "Error", MB_OK);
	}

	CTextureManager::GetInstance().SetTexture("ShadowTexture", srv, nullptr);

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

	hr = device->CreateTexture2D(&descDepth, nullptr, &m_dSTexture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateTexture2D error", "error", MB_OK);
		return;
	}

	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	hr = device->CreateDepthStencilView(m_dSTexture, nullptr, &m_dSTexDSV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateDepthStencilView error", "error", MB_OK);
		return;
	}

	// 定数バッファ生成
	bool sts = CreateConstantBufferWrite(
		device,
		sizeof(tagConstantShadowBuffer),
		&m_constantShadowBuffer); // シャドウマップ用定数バッファ

	if (!sts)
	{
		MessageBox(nullptr, "CreateConstantBufferWrite error", "error", MB_OK);
		return;
	}
}

//================================================================================================
//================================================================================================

void CShadowManager::DrawShadowMap()
{
	// ターゲットバッファクリア
	float ClearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// デバイスコンテキスト取得
	ID3D11DeviceContext *devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();

	// レンダリングターゲットビュー、デプスステンシルビューを設定
	ID3D11RenderTargetView *rtv[] = {m_shadowTarget};
	devcontext->OMSetRenderTargets(1, rtv, m_dSTexDSV);

	// ビューポートを設定
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(m_depthWidth);
	vp.Height = static_cast<float>(m_depthHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	devcontext->RSSetViewports(1, &vp);

	// ターゲットバッファクリア
	devcontext->ClearRenderTargetView(m_shadowTarget, ClearColor);
	// Zバッファクリア
	devcontext->ClearDepthStencilView(m_dSTexDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//各シェーダーのセット
	CShaderManager::GetInstance().SetVertexShader("DefaultVertex/VSShadowMap.fx");
	CShaderManager::GetInstance().SetPixelShader("DefaultPixel/PSShadowMap.fx");

	//車道マップではライト目線のカメラを用意する必要があるのでライト座標取得でやる
	DirectX::XMFLOAT3 lightpos;

	//ライトの座標(ぢレクションライトの方向)を取得 & 正規化
	auto [light_x, light_y, light_z] = m_comLight->m_gameObject->m_transform->m_worldPosition.GetValue();
	lightpos = DirectX::XMFLOAT3(light_x, light_y, light_z);

	//車道マップを生成するために光源からみたカメラを生成する
	ALIGN16 DirectX::XMVECTOR eye = DirectX::XMVectorSet(lightpos.x, lightpos.y, lightpos.z, 0.0f);
	ALIGN16 DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ALIGN16 DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	ALIGN16 DirectX::XMMATRIX camera;
	camera = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, at, up));

	// ビュー変換行列をセット（光源位置からのカメラ）
	tagConstantShadowBuffer cb;
	DirectX::XMStoreFloat4x4(&cb.ViewFromLight, camera);

	// 光源カメラ用のプロジェクション変換行列
	float nearclip = 10.0f;
	float farclip = 300.0f; // ファークリップの値がおかしくてバグった
	float Aspect = 1.0f;
	float Fov = DirectX::XM_PI / 3;

	ALIGN16 DirectX::XMMATRIX ProjectionFromLight;

	ProjectionFromLight = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(Fov, Aspect, nearclip, farclip));
	DirectX::XMStoreFloat4x4(&cb.ProjectionFromLight, ProjectionFromLight); // プロジェクション変換行列をセット

	// スクリーン座標をＵＶ座標に変換する行列
	ALIGN16 DirectX::XMMATRIX mtxtoscreen = DirectX::XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	// スクリーン座標をＵＶ座標に変換する行列
	DirectX::XMStoreFloat4x4(&cb.ScreenToUVCoord, DirectX::XMMatrixTranspose(mtxtoscreen));

	// 定数バッファに反映させる
	D3D11_MAPPED_SUBRESOURCE pData;

	//上書き？
	HRESULT hr = devcontext->Map(m_constantShadowBuffer,
								 0,
								 D3D11_MAP_WRITE_DISCARD,
								 0,
								 &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, (void *)(&cb), sizeof(tagConstantShadowBuffer));
		devcontext->Unmap(m_constantShadowBuffer, 0);
	}

	// 定数バッファを8番スロットにセット
	devcontext->PSSetConstantBuffers(8, 1, &m_constantShadowBuffer);
	devcontext->VSSetConstantBuffers(8, 1, &m_constantShadowBuffer);

	//影を描画したいオブジェクトの描画処理を行いテクスチャを生成
	for (auto itr : m_listObjectDrawFunction)
	{
		itr.second();
	}
}
