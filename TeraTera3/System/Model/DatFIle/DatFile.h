//*****************************************************************************
//!	@file	CDirect3DXFile.h
//!	@brief
//!	@note	datファイルクラス
//!	@author
//*****************************************************************************
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "../DatFileLoader/DatFileLoader.h"

namespace X
{

	// 定数バッファ定義（マテリアル単位）
	struct ConstantBufferMaterial
	{
		DirectX::XMFLOAT4 AmbientMaterial;	// 環境光の反射率
		DirectX::XMFLOAT4 DiffuseMaterial;	// ディフューズ光の反射率
		DirectX::XMFLOAT4 SpecularMaterial; // スペキュラ光の反射率
	};
}

/**
 * @brief Xファイルクラス
 */
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

	/**
	 * @brief datファイルの読み込みをする処理
	 * @param xfilename 
	 * @param device11 datファイル名
	 * @param device11Context デバイスオブジェクト
	 * @return true 成功
	 * @return false 失敗
	 */
	bool Load(const char *xfilename, ID3D11Device *device11, ID3D11DeviceContext *device11Context);

	/**
	 * @brief 破棄する処理
	 */
	void UnLoad();

	/**
	 * @brief Xファイル描画する処理
	 * @param device デバイスオブジェクト
	 */
	void Draw(ID3D11DeviceContext *device);

	/**
	 * @brief ローダーの取得
	 * @return DatFileLoader* 取得したいもののポインタ
	 */
	DatFileLoader *GetDatFileLoader()
	{
		return m_datfile;
	}
};
