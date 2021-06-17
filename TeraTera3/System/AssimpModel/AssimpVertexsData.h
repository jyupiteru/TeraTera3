/**
 * @file AssimpVertexsData.h
 * @author jupiter ()
 * @brief Assimpを使用してモデルを読み込むときに使用する各種データ
 */
#pragma once
#include <directxmath.h>
#include <assimp/Importer.hpp>

typedef int32_t VertexSuu; // 頂点数

/**
 * @brief Assimpモデルをシェーダーに渡す際に使用する構造体
 */
struct tagAssimpVertex
{
	DirectX::XMFLOAT3 m_Pos;	//	位置
	DirectX::XMFLOAT3 m_Normal; //	法線
	DirectX::XMFLOAT2 m_Tex;	//	UV座標
	int m_BoneIndex[4];			//	本来はボーンインデックス
	float m_BoneWeight[4];		//	ウェイト値
	int m_BoneNum;				//	ボーン数
	std::string m_BoneName[4];	//	本来はボーンインデックスで管理するべき
};

typedef int32_t IndexSuu; // インデックスの数。
typedef uint32_t Index;	  // インデックス。

// マテリアル
struct tagMaterial
{
	float m_Alpha;
	DirectX::XMFLOAT3 m_Diffuse;
	DirectX::XMFLOAT3 m_Specular;
	float m_SpecularPower;
	DirectX::XMFLOAT3 m_Ambient;
	DirectX::XMFLOAT3 m_Emissive;

	tagMaterial()
		: m_Alpha(0),
		  m_Diffuse(DirectX::XMFLOAT3(0, 0, 0)),
		  m_Specular(DirectX::XMFLOAT3(0, 0, 0)),
		  m_SpecularPower(0),
		  m_Ambient(DirectX::XMFLOAT3(0, 0, 0)),
		  m_Emissive(DirectX::XMFLOAT3(0, 0, 0))
	{
	}
};

// ボーン情報
struct tagBONE
{
	//std::string Name;
	unsigned int idx = -1;		 // ボーン配列のインデックス番号
	aiMatrix4x4 Matrix;			 // 初期配置行列
	aiMatrix4x4 AnimationMatrix; // ボーン行列
	aiMatrix4x4 OffsetMatrix;	 // ボーンオフセット行列
};

struct tagConstantBufferBoneMatrix // ボーン行列と色情報格納用のコンスタントバッファ
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4X4 mBoneMatrix[400];
};

// 定数バッファ定義（マテリアル）
struct tagConstantBufferMaterial
{
	DirectX::XMFLOAT4 AmbientMaterial;	// 環境光の反射率
	DirectX::XMFLOAT4 DiffuseMaterial;	// ディフューズ光の反射率
	DirectX::XMFLOAT4 SpecularMaterial; // スペキュラ光の反射率
};