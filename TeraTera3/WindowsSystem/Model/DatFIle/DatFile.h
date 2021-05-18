//*****************************************************************************
//!	@file	CDirect3DXFile.h
//!	@brief
//!	@note	datファイルクラス
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <d3d11.h>
#include <DirectXMath.h>
#include "../DatFileLoader/DatFileLoader.h"

namespace X
{
	// 頂点構造体定義
	//	struct Vertex
	//	{
	//		XMFLOAT3 Pos;						// XNA MATHライブラリで使用できるように定義された構造体　float*3
	//		XMFLOAT3 Normal;
	//		XMFLOAT2 Tex;						// テクスチャ座標
	//	};

	// 定数バッファ定義（マテリアル単位）
	struct ConstantBufferMaterial
	{
		DirectX::XMFLOAT4 AmbientMaterial;	// 環境光の反射率
		DirectX::XMFLOAT4 DiffuseMaterial;	// ディフューズ光の反射率
		DirectX::XMFLOAT4 SpecularMaterial; // スペキュラ光の反射率
	};
} // namespace X

//=============================================================================
//!	@class	CDirect3DXFile
//!	@brief	Xファイルクラス
//=============================================================================
class DatFile
{
private:
	DWORD m_nummaterial = 0;					// マテリアル数
	ID3D11Buffer *m_pVertexBuffer = nullptr;	// 頂点バッファ
	ID3D11Buffer *m_pIndexBuffer;				// インデックスバッファ
	DatFileLoader *m_datfile = nullptr;			// datファイルデータ
	ID3D11ShaderResourceView **m_srv = nullptr; // シェーダーリソースビュー
	ID3D11Buffer *m_cb3 = nullptr;				// コンスタントバッファ(マテリアル用)
	ID3D11Resource **m_texres = nullptr;		// テクスチャリソース
public:
	DatFile()
	{
		m_pVertexBuffer = nullptr;
		m_pIndexBuffer = nullptr;
	}

	virtual ~DatFile()
	{
		UnLoad();
	}
	bool Load(const char *xfilename, ID3D11Device *device11, ID3D11DeviceContext *device11Context);
	void UnLoad();
	void Draw(ID3D11DeviceContext *device);
	DatFileLoader *GetDatFileLoader()
	{
		return m_datfile;
	}
};

//******************************************************************************
//	End of file.
//******************************************************************************