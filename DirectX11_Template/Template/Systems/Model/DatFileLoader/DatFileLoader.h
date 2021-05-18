#pragma once

#include <stdint.h>
#include <string>
#include <DirectXMath.h>

#include "../../Memory.h"
#include "../../NonCopyable.h"

namespace X
{
	typedef int32_t VertexSuu; // 頂点数
	struct Vertex
	{
		DirectX::XMFLOAT3 m_Pos;	// 位置
		DirectX::XMFLOAT3 m_Normal; // 法線
		DirectX::XMFLOAT2 m_Tex;	// テクスチャ座標
	};

	typedef int32_t IndexSuu; // インデックスの数。
	typedef uint32_t Index;	  // インデックス。

	typedef int32_t MaterialSuu; // マテリアルの数
	struct Material
	{
		float m_Alpha;
		DirectX::XMFLOAT3 m_Diffuse;
		DirectX::XMFLOAT3 m_Specular;
		float m_SpecularPower;
		DirectX::XMFLOAT3 m_Ambient;
		DirectX::XMFLOAT3 m_Emissive;

		Material()
			: m_Alpha(0),
			  m_Diffuse(DirectX::XMFLOAT3(0, 0, 0)),
			  m_Specular(DirectX::XMFLOAT3(0, 0, 0)),
			  m_SpecularPower(0),
			  m_Ambient(DirectX::XMFLOAT3(0, 0, 0)),
			  m_Emissive(DirectX::XMFLOAT3(0, 0, 0))
		{
		}
	};

	typedef int32_t SubsetSuu; // サブセットの数
	struct Subset
	{
		// マテリアルテーブルのインデックス。
		// 使わないなら-1。
		int32_t m_MaterialIdx; // マテリアルのインデックス

		int32_t m_VertexSuu; // 頂点数

		unsigned int m_FaceStart = 0;	// 開始面
		unsigned int m_FaceCount = 0;	// 面数
		unsigned int m_VertexStart = 0; // 開始頂点
		unsigned int m_VertexCount = 0; // 頂点の数

		Subset()
			: m_MaterialIdx(-1),
			  m_VertexSuu(0)
		{
		}
	};
} // namespace X

class DatFileLoader : private NonCopyable
{
public:
	X::VertexSuu m_VertexSuu = 0;
	X::Vertex *m_Vertex = nullptr;

	X::IndexSuu m_IndexSuu = 0;
	X::Index *m_Index = nullptr;

	X::MaterialSuu m_MaterialSuu = 0;
	X::Material *m_Material = nullptr;
	std::string *m_DecalTex = nullptr; // マテリアル配列と対応。なければ空文字列。

	X::SubsetSuu m_SubsetSuu = 0;
	X::Subset *m_Subset = nullptr;

public:
	explicit DatFileLoader() {}
	~DatFileLoader();
	void Serialize(const char *filename);
	bool Deserialize(const char *filename);
};