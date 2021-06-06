/**
 * @file ComSphere.cpp
 * @author jupiter ()
 * @brief ComSphereの実装が書かれたcpp
 */

#include "ComSphere.h"
#include "../../DefaultComponents.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"

using namespace DirectX;

const float PI = DirectX::XM_PI;

ID3D11Buffer *ComSphere::m_pVertexBuffer = nullptr;

ID3D11Buffer *ComSphere::m_pIndexBuffer = nullptr;

unsigned int ComSphere::m_classCounter = 0;

void ComSphere::Init()
{
	m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT3D);
	m_divX.SetValue(15);
	m_divY.SetValue(15);
}

//================================================================================================
//================================================================================================

void ComSphere::Ready()
{
	// 球のインデックスデータを作成
	CreateIndex();

	// 球の頂点データを作成
	CreateVertex();

	if (m_pIndexBuffer == nullptr)
	{
		//本当はInitの処理 分割数を動的?に決めれるようにするためにここに
		ID3D11Device *device = GetDX11Device();

		int sts;

		// インデックスバッファ作成
		sts = CreateIndexBuffer(
			device,
			(m_divY.GetValue()) * (m_divX.GetValue()) * 2 * 3, // インデックス数
			m_face,											   // インデックスデータ先頭アドレス
			&m_pIndexBuffer);								   // インデックスバッファ
		if (!sts)
		{
			MessageBox(NULL, TEXT("CreateBuffer(index buffer) error"), TEXT("Error"), MB_OK);
		}
	}
	m_classCounter++;

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	unsigned int numElements = ARRAYSIZE(layout);

	//専用シェーダの読み込み
	m_pComShader = m_gameObject->GetComponent<ComShader>();
	if (m_pComShader == nullptr)
	{
		m_pComShader = m_gameObject->AddComponent<ComShader>();
	}
	m_pComShader->LoadVertexShader("vs3dshape.fx", layout, numElements, true);
	m_pComShader->LoadPixelShader("ps3dshape.fx", true);
}

//================================================================================================
//================================================================================================

void ComSphere::Uninit()
{

	if (m_vertex)
	{
		delete[] m_vertex;
	}

	if (m_face)
	{
		delete[] m_face;
	}

	m_classCounter--;
	if (m_classCounter <= 0)
	{
		if (m_pIndexBuffer)
		{
			m_pIndexBuffer->Release();
			m_pIndexBuffer = nullptr;
		}

		if (m_pVertexBuffer)
		{
			m_pVertexBuffer->Release();
			m_pVertexBuffer = nullptr;
		}
	}
}

//================================================================================================
//================================================================================================

void ComSphere::Draw()
{
	ChangeColors();

	XMFLOAT4X4 mtx = m_gameObject->m_transform->GetMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

	m_pComShader->SetPixelShader();
	m_pComShader->SetVertexShader();

	ID3D11DeviceContext *device = GetDX11DeviceContext();
	// 頂点バッファをセットする
	unsigned int stride = sizeof(tagVertex);
	unsigned offset = 0;
	device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	   // インデックスバッファをセット
	device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // トポロジーをセット（旧プリミティブタイプ）

	device->DrawIndexed(m_facenum * 3, // 描画するインデックス数（面数×３）
						0,			   // 最初のインデックスバッファの位置
						0);			   // 頂点バッファの最初から使う
}

//================================================================================================
//================================================================================================

void ComSphere::ImGui_Draw(unsigned int windowid)
{
	ImGui::BulletText("Radius : %0.1f", m_radius);
	ImGui::BulletText("Division");
	ImGui::Indent();
	ImGui::Text("X : %0.1u, Y : %0.1u", m_divX.GetValue(), m_divY.GetValue());
	ImGui::Unindent();
}

//================================================================================================
//================================================================================================

void ComSphere::CreateIndex()
{

	// 面数セット
	m_facenum = m_divY.GetValue() * m_divX.GetValue() * 2;

	m_face = new Face[m_facenum];

	Face *pface = m_face;

	// インデックス生成
	for (unsigned int y = 0; y < m_divY.GetValue(); y++)
	{
		for (unsigned int x = 0; x < m_divX.GetValue(); x++)
		{
			int count = (m_divX.GetValue() + 1) * y + x; // 左上座標のインデックス

			// 上半分
			pface->idx[0] = count;
			pface->idx[1] = count + 1;
			pface->idx[2] = count + 1 + (m_divX.GetValue() + 1);

			pface++;

			// 下半分
			pface->idx[0] = count;
			pface->idx[1] = count + (m_divX.GetValue() + 1) + 1;
			pface->idx[2] = count + (m_divX.GetValue() + 1);

			pface++;
		}
	}
}

//================================================================================================
//================================================================================================

void ComSphere::CreateVertex()
{

	float azimuth = 0.0f;	// 方位角
	float elevation = 0.0f; // 仰角

	m_vertex = new tagVertex[(m_divX.GetValue() + 1) * (m_divY.GetValue() + 1)];
	tagVertex *pvtx = m_vertex;

	XMFLOAT3 Normal;
	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= m_divY.GetValue(); y++)
	{
		elevation = (PI * (float)y) / (float)m_divY.GetValue(); // 仰角をセット
		float r = m_radius * sinf(elevation);					// 仰角に応じた半径を計算

		for (unsigned int x = 0; x <= m_divX.GetValue(); x++)
		{
			azimuth = (2 * PI * (float)x) / (float)m_divX.GetValue(); // 方位角をセット

			// 頂点座標セット
			pvtx->Pos.x = r * cosf(azimuth);
			pvtx->Pos.y = m_radius * cosf(elevation);
			pvtx->Pos.z = r * sinf(azimuth);

			// 法線ベクトルセット
			Normalize(pvtx->Pos, Normal); // 法線を計算
			pvtx->Normal = Normal;		  // 法線をセット

			// 次へ
			pvtx++;
		}
	}
	ChangeColors();
}

//================================================================================================
//================================================================================================

void ComSphere::ChangeColors()
{
	tagVertex *pvtx = m_vertex;

	//todo 効率悪いのでここの最適化処理
	for (unsigned int y = 0; y <= m_divY.GetValue(); y++)
	{ // 仰角に応じた半径を計算

		for (unsigned int x = 0; x <= m_divX.GetValue(); x++)
		{
			std::tie(pvtx->Color.x,
					 pvtx->Color.y,
					 pvtx->Color.z,
					 pvtx->Color.w) = m_gameObject->m_transform->m_color.GetValue();

			pvtx++;
		}
	}

	auto vertices = (m_divY.GetValue() + 1) * (m_divX.GetValue() + 1);

	if (m_pVertexBuffer == nullptr)
	{
		// 頂点バッファ作成（後で変更可能なもの）
		bool sts = CreateVertexBufferWrite(GetDX11Device(),	  //デバイス
										   sizeof(tagVertex), //ストライド（1頂点当たりのバイト数）
										   vertices,		  //頂点数
										   m_vertex,		  //初期化データの先頭アドレス
										   &m_pVertexBuffer); //頂点バッファ
		if (!sts)
		{
			MessageBox(nullptr, "ComBox ChangeColor Error", "error", MB_OK);
		}
	}
	else
	{
		// 作成済みなら 内容を書き換える
		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = GetDX11DeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr))
		{
			memcpy_s(pData.pData, pData.RowPitch, (void *)(m_vertex), sizeof(tagVertex) * vertices);
			GetDX11DeviceContext()->Unmap(m_pVertexBuffer, 0);
		}
	}
}

//================================================================================================
//================================================================================================

void ComSphere::Normalize(XMFLOAT3 vector, XMFLOAT3 &Normal)
{

	DirectX::XMVECTOR v;
	v = XMLoadFloat3(&vector); // XMFLOAT3=>XMVECTOR
	v = XMVector3Normalize(v); // 正規化
	XMStoreFloat3(&Normal, v); // XMVECTOR=>XMFLOAT3
}