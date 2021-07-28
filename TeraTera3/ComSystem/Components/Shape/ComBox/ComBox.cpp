/**
 * @file ComBox.cpp
 * @author jupiter ()
 * @brief ComBoxの実装部分が記載されたcpp
**/

#include "ComBox.h"
#include "../../DefaultComponents.h"

#include "../../../../System/CDirectXGraphics/CDirectxGraphics.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../Behavior/ComTransform/ComTransform.h"
#include "../../../../Managers/ShadowManager/CShadowManager.h"

using namespace DirectX;

const float PI = DirectX::XM_PI;

ID3D11Buffer *ComBox::m_pVertexBuffer = nullptr;

ID3D11Buffer *ComBox::m_pIndexBuffer = nullptr;

unsigned int ComBox::m_comCount = 0;

ComBox::tagVertex ComBox::m_vertex[8];

void ComBox::Init()
{
	m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT3D);

	bool sts;

	if (m_pIndexBuffer == nullptr)
	{
		// BOXのインデックスデータを作成
		CreateIndex();

		// BOXの頂点データを作成
		CreateVertex();
		ID3D11Device *device = CDirectXGraphics::GetInstance().GetDXDevice();
		// インデックスバッファ作成
		sts = CreateIndexBuffer(
			device,
			12 * 3,			  // １２面×３頂点
			m_face,			  // インデックスデータ先頭アドレス
			&m_pIndexBuffer); // インデックスバッファ
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		}
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	unsigned int numelements = ARRAYSIZE(layout);

	//専用シェーダの読み込み
	m_pComShader = m_gameObject->GetComponent<ComShader>();
	if (m_pComShader == nullptr)
	{
		m_pComShader = m_gameObject->AddComponent<ComShader>();
	}
	m_pComShader->LoadVertexShader("VS3dShape.fx", layout, numelements, true);
	m_pComShader->LoadPixelShader("PSOnlyColor.fx", true);

	m_comCount++;
}

//================================================================================================
//================================================================================================

void ComBox::Ready()
{
	if (m_flagDrawShadow)
	{ //影の描画対象なので関数をセットする
		CShadowManager::GetInstance().SetDrawShadowFuction(m_gameObject->m_objectName, std::bind(&ComBox::DrawShadow, this));
	}
}

//================================================================================================
//================================================================================================

void ComBox::Uninit()
{
	m_comCount--;
	if (m_comCount <= 0)
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

	CShadowManager::GetInstance().RemoveDrawFunction(this->m_gameObject->m_objectName);
}

//================================================================================================
//================================================================================================

void ComBox::ImGuiDraw(unsigned int windowid)
{
	ImGui::BulletText("LocalSize");
	ImGui::Indent();
	ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", m_lengthx, m_lengthy, m_lengthz);
	ImGui::Unindent();
}

//================================================================================================
//================================================================================================

void ComBox::Draw()
{
	ChangeColor();

	m_pComShader->SetPixelShader();
	m_pComShader->SetVertexShader();

	DrawShadow();
}

//================================================================================================
//================================================================================================

void ComBox::CreateIndex()
{
	m_face[0].idx[0] = 0; // 手前  ok
	m_face[0].idx[1] = 1;
	m_face[0].idx[2] = 2;

	m_face[1].idx[0] = 2;
	m_face[1].idx[1] = 3;
	m_face[1].idx[2] = 0;

	m_face[2].idx[0] = 5; // 奥  ok
	m_face[2].idx[1] = 4;
	m_face[2].idx[2] = 7;

	m_face[3].idx[0] = 7;
	m_face[3].idx[1] = 6;
	m_face[3].idx[2] = 5;

	m_face[4].idx[0] = 4; // 左側 ok
	m_face[4].idx[1] = 0;
	m_face[4].idx[2] = 7;

	m_face[5].idx[0] = 0;
	m_face[5].idx[1] = 3;
	m_face[5].idx[2] = 7;

	m_face[6].idx[0] = 1; // 右側 ok
	m_face[6].idx[1] = 5;
	m_face[6].idx[2] = 6;

	m_face[7].idx[0] = 1;
	m_face[7].idx[1] = 6;
	m_face[7].idx[2] = 2;

	m_face[8].idx[0] = 0; // 上側 ok
	m_face[8].idx[1] = 4;
	m_face[8].idx[2] = 1;

	m_face[9].idx[0] = 4;
	m_face[9].idx[1] = 5;
	m_face[9].idx[2] = 1;

	m_face[10].idx[0] = 3; // 下側
	m_face[10].idx[1] = 2;
	m_face[10].idx[2] = 7;

	m_face[11].idx[0] = 6;
	m_face[11].idx[1] = 7;
	m_face[11].idx[2] = 2;
}

//================================================================================================
//================================================================================================

void ComBox::CreateVertex()
{

	m_vertex[0].Pos.x = -(m_lengthx / 2.0f);
	m_vertex[0].Pos.y = (m_lengthy / 2.0f);
	m_vertex[0].Pos.z = -(m_lengthz / 2.0f);

	m_vertex[1].Pos.x = (m_lengthx / 2.0f);
	m_vertex[1].Pos.y = (m_lengthy / 2.0f);
	m_vertex[1].Pos.z = -(m_lengthz / 2.0f);

	m_vertex[2].Pos.x = (m_lengthx / 2.0f);
	m_vertex[2].Pos.y = -(m_lengthy / 2.0f);
	m_vertex[2].Pos.z = -(m_lengthz / 2.0f);

	m_vertex[3].Pos.x = -(m_lengthx / 2.0f);
	m_vertex[3].Pos.y = -(m_lengthy / 2.0f);
	m_vertex[3].Pos.z = -(m_lengthz / 2.0f);

	m_vertex[4].Pos.x = -(m_lengthx / 2.0f);
	m_vertex[4].Pos.y = (m_lengthy / 2.0f);
	m_vertex[4].Pos.z = (m_lengthz / 2.0f);

	m_vertex[5].Pos.x = (m_lengthx / 2.0f);
	m_vertex[5].Pos.y = (m_lengthy / 2.0f);
	m_vertex[5].Pos.z = (m_lengthz / 2.0f);

	m_vertex[6].Pos.x = (m_lengthx / 2.0f);
	m_vertex[6].Pos.y = -(m_lengthy / 2.0f);
	m_vertex[6].Pos.z = (m_lengthz / 2.0f);

	m_vertex[7].Pos.x = -(m_lengthx / 2.0f);
	m_vertex[7].Pos.y = -(m_lengthy / 2.0f);
	m_vertex[7].Pos.z = (m_lengthz / 2.0f);

	DirectX::XMFLOAT3 Normal;

	for (int i = 0; i < 8; i++)
	{
		// 法線ベクトルセット
		Normalize(m_vertex[i].Pos, Normal); // 法線を計算
		m_vertex[i].Normal = Normal;		// 法線をセット
	}

	ChangeColor();
}

//================================================================================================
//================================================================================================

void ComBox::ChangeColor()
{
	for (int i = 0; i < 8; i++)
	{
		std::tie(m_vertex[i].Color.x,
				 m_vertex[i].Color.y,
				 m_vertex[i].Color.z,
				 m_vertex[i].Color.w) = m_gameObject->m_transform->m_color.GetValue();
	}

	if (m_pVertexBuffer == nullptr)
	{
		// 頂点バッファ作成（後で変更可能なもの）
		bool sts = CreateVertexBufferWrite(CDirectXGraphics::GetInstance().GetDXDevice(), //デバイス
										   sizeof(tagVertex),							  //ストライド（1頂点当たりのバイト数）
										   8,											  //頂点数
										   m_vertex,									  //初期化データの先頭アドレス
										   &m_pVertexBuffer);							  //頂点バッファ
		if (!sts)
		{
			MessageBox(nullptr, "ComBox ChangeColor Error", "error", MB_OK);
		}
	}
	else
	{
		// 作成済みなら 内容を書き換える
		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = CDirectXGraphics::GetInstance().GetImmediateContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr))
		{
			memcpy_s(pData.pData, pData.RowPitch, (void *)(m_vertex), sizeof(tagVertex) * 8);
			CDirectXGraphics::GetInstance().GetImmediateContext()->Unmap(m_pVertexBuffer, 0);
		}
	}
}

//================================================================================================
//================================================================================================

void ComBox::Normalize(XMFLOAT3 vector, XMFLOAT3 &Normal)
{

	DirectX::XMVECTOR v;
	v = XMLoadFloat3(&vector); // XMFLOAT3=>XMVECTOR
	v = XMVector3Normalize(v); // 正規化
	XMStoreFloat3(&Normal, v); // XMVECTOR=>XMFLOAT3
}

//================================================================================================
//================================================================================================

void ComBox::DrawShadow()
{
	XMFLOAT4X4 mtx = m_gameObject->m_transform->GetMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

	ID3D11DeviceContext *devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();
	// 頂点バッファをセットする
	unsigned int stride = sizeof(tagVertex);
	unsigned offset = 0;
	devcontext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	devcontext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	   // インデックスバッファをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // トポロジーをセット（旧プリミティブタイプ）

	devcontext->DrawIndexed(m_facenum * 3, // 描画するインデックス数（面数×３）
							0,			   // 最初のインデックスバッファの位置
							0);			   // 頂点バッファの最初から使う
}