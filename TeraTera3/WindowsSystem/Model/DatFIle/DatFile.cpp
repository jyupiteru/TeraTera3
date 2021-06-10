﻿
#include "DatFile.h"
#include "../../Shader/Shader.h"
#include "WICTextureLoader.h"

using namespace DirectX;

bool DatFile::Load(const char *filename, ID3D11Device *device11, ID3D11DeviceContext *device11Context)
{
	bool sts;

	// datファイル読み込み
	m_datfile = new DatFileLoader();
	sts = m_datfile->Deserialize(filename);
	if (!sts)
	{
		return false;
	}

	// 頂点バッファ作成
	sts = CreateVertexBuffer(
		device11,
		sizeof(X::Vertex),		// １頂点当たりバイト数
		m_datfile->m_VertexSuu, // 頂点数
		m_datfile->m_Vertex,	// 頂点データ格納メモリ先頭アドレス
		&m_pVertexBuffer);		// 頂点バッファ
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// インデックスバッファ作成
	sts = CreateIndexBuffer(
		device11,
		m_datfile->m_IndexSuu, // 頂点数
		m_datfile->m_Index,	   // 頂点データ格納メモリ先頭アドレス
		&m_pIndexBuffer);	   // インデックスバッファ
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// コンスタントバッファ作成
	sts = CreateConstantBuffer(
		device11,
		sizeof(X::ConstantBufferMaterial),
		&m_cb3);
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(constant buffer Material) error", "Error", MB_OK);
		return false;
	}

	// シェーダーリソースビュー作成
	m_srv = new ID3D11ShaderResourceView *[m_datfile->m_MaterialSuu]; //  m_xfile->m_MaterialSuu];
	m_texres = new ID3D11Resource *[m_datfile->m_MaterialSuu];

	for (int i = 0; i < m_datfile->m_MaterialSuu; i++)
	{
		if (m_datfile->m_DecalTex[i] != "null" && m_datfile->m_DecalTex[i] != "none")
		{
			// テクスチャ読み込み
			bool sts = CreateSRVfromFile(m_datfile->m_DecalTex[i].c_str(), device11, device11Context, &m_texres[i], &m_srv[i]);
			if (!sts)
			{
				std::string str;
				str = "CreateSRVfromFile " + m_datfile->m_DecalTex[i];
				MessageBox(NULL, str.c_str(), "Error", MB_OK);
			}
		}
		else
		{
			// テクスチャ読み込み
			bool sts = CreateSRVfromWICFile("assets/white.bmp", // WICファイルからシェーダーリソースビューを作成する
											device11,
											device11Context,
											&m_texres[i],
											&m_srv[i]);
		}
	}

	return true;
}

//================================================================================================
//================================================================================================

void DatFile::UnLoad()
{

	if (m_datfile != nullptr)
	{
		for (int i = 0; i < m_datfile->m_MaterialSuu; i++)
		{
			if (m_srv[i])
			{
				m_srv[i]->Release();
			}
			if (m_texres[i])
			{
				m_texres[i]->Release();
			}
		}
		delete m_datfile;
	}

	if (m_srv != nullptr)
	{
		delete[] m_srv;
	}

	if (m_texres != nullptr)
	{
		delete[] m_texres;
	}

	if (m_pIndexBuffer != nullptr)
	{ // インデックスバッファ解放
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if (m_pVertexBuffer != nullptr)
	{ // 頂点バッファ解放
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_cb3 != nullptr)
	{ // コンスタントバッファ解放
		m_cb3->Release();
		m_cb3 = nullptr;
	}
}

/**
 * @brief 
 * @param device 
 */
void DatFile::Draw(ID3D11DeviceContext *device)
{
	int indexnum = 0;
	int indexstart = 0;

	X::ConstantBufferMaterial cb; // コンスタントバッファ３用構造体（マテリアルの値）

	// 頂点バッファをセットする
	unsigned int stride = sizeof(X::Vertex);
	unsigned offset = 0;
	device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// インデックスバッファをセット
	device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // トポロジーをセット（旧プリミティブタイプ）

	indexstart = indexstart + indexnum; // インデックスの開始位置

	for (int i = 0; i < m_datfile->m_MaterialSuu; i++)
	{
		int subsetidx = -1;
		// マテリアル数分セット
		device->PSSetShaderResources(0, 1, &m_srv[i]);

		// マテリアルに該当するサブセットを取得する
		for (int j = 0; j < m_datfile->m_SubsetSuu; j++)
		{
			if (i == m_datfile->m_Subset[j].m_MaterialIdx)
			{

				cb.AmbientMaterial.x = m_datfile->m_Material[i].m_Ambient.x; // 環境光の反射率
				cb.AmbientMaterial.y = m_datfile->m_Material[i].m_Ambient.y;
				cb.AmbientMaterial.z = m_datfile->m_Material[i].m_Ambient.z;
				cb.AmbientMaterial.w = 1.0f;

				cb.DiffuseMaterial.x = m_datfile->m_Material[i].m_Diffuse.x; // ディフューズ光の反射率
				cb.DiffuseMaterial.y = m_datfile->m_Material[i].m_Diffuse.y;
				cb.DiffuseMaterial.z = m_datfile->m_Material[i].m_Diffuse.z;
				cb.DiffuseMaterial.w = 1.0f;

				//				cb.SpecularMaterial.x = m_xfile->m_Material[i].m_Specular.x;				// スペキュラ光の反射率
				//				cb.SpecularMaterial.y = m_xfile->m_Material[i].m_Specular.y;
				//				cb.SpecularMaterial.z = m_xfile->m_Material[i].m_Specular.z;
				cb.SpecularMaterial.w = 1.0f;

				cb.SpecularMaterial.x = m_datfile->m_Material[i].m_Diffuse.x; // スペキュラ光の反射率
				cb.SpecularMaterial.y = m_datfile->m_Material[i].m_Diffuse.y;
				cb.SpecularMaterial.z = m_datfile->m_Material[i].m_Diffuse.z;

				// 定数バッファ更新
				device->UpdateSubresource(
					m_cb3, // コンスタントバッファ
					0,	   // コピー先サブリソース
					NULL,  // サブリソースを定義するボックス　ＮＵＬＬの場合オフセットなし
					&cb,   // コピー元データ
					0,	   // 1行のサイズ
					0);	   // 1深度スライスのサイズ

				// 定数バッファ3をピクセルシェーダーへセット
				device->VSSetConstantBuffers(
					3,		 // スタートスロット
					1,		 // 個数
					&m_cb3); // コンスタントバッファ

				// 定数バッファ3をピクセルシェーダーへセット
				device->PSSetConstantBuffers(
					3,		 // スタートスロット
					1,		 // 個数
					&m_cb3); // コンスタントバッファ

				subsetidx = j;
				device->DrawIndexed(m_datfile->m_Subset[subsetidx].m_VertexSuu,		// 描画するインデックス数(Face*3)
									m_datfile->m_Subset[subsetidx].m_FaceStart * 3, // 最初のインデックスバッファの位置
									0);												// 頂点バッファの最初から使う
			}
		}
	}
}
