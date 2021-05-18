#pragma once

#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include <map>
#include "../vertexproto.h"

// テクスチャ構造体
struct Texture
{
	std::string type;				   // テクスチャ種類
	std::string path;				   // テクスチャパス名
	ID3D11Resource *resource;		   // リソース
	ID3D11ShaderResourceView *texture; // SRV
};

class Mesh
{
public:
	std::vector<Vertex> m_vertices;		 // 頂点データ
	std::vector<unsigned int> m_indices; // インデックス
	std::vector<Texture> m_textures;	 // テクスチャ
	ID3D11Device *m_dev;				 // デバイス

	std::unordered_map<std::string, std::vector<DirectX::XMFLOAT3>> m_bonevertices; // ボーン毎の頂点データ

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mtrl);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Draw(ID3D11DeviceContext *devcon);
	void Close();

private:
	ID3D11Buffer *m_VertexBuffer; // 頂点バッファ
	ID3D11Buffer *m_IndexBuffer;  // インデックスバッファ
	ID3D11Buffer *m_cbmtrl;		  // 定数バッファ
	Material m_mtrl;			  // マテリアル

	bool setupMesh();
	void updatevertexbuffer();
};