#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <assimp\scene.h>
#include <map>
#include <assimp\Importer.hpp>
//#include "dx11mathutil.h"
#include "../Mesh/mesh.h"
#include "../../../CDirectXGraphics/CDirectXGraphics.h"
#include "../animationdataassimp/animationdataassimp.h"

class Com3DModelAssimp;

/**
 * @brief アニメーションの際に必要になる各種データ
 * @todo これは一時的(再編成が大変)なので絶対に無くすこと！
 */
struct tagAssimpAnimationData
{
	int m_frame = 0; // フレーム番号

	int m_maxflame = 0; //アニメーションの最大フレーム数

	float m_TickPerSecond;

	int m_preFrame = 0;	   // キーフレーム補間用
	int m_cnt = 0;		   // キーフレーム補間用
	float m_factor = 0.0f; // キーフレーム補間用

	std::map<std::string, BONE> m_listBone; //ボーンデータ（ノードの名前で参照）
};

class ModelData
{

public:
	friend Com3DModelAssimp;

	ModelData();
	~ModelData();

	bool Load(std::string resourcefolder, std::string filename);

	void Update(const DirectX::XMFLOAT4X4 &mtxworld,
				const aiScene *animationscene,
				unsigned int animno,
				tagAssimpAnimationData &animationdata);

	void Draw(ID3D11DeviceContext *devcon,
			  DirectX::XMFLOAT4X4 &mtxworld,
			  tagAssimpAnimationData &animationdata);

	void Exit();

	const std::vector<Mesh> &GetMeshes() const
	{
		return m_meshes;
	}

	void LoadMaterial();

	/**
	 * @brief アニメーションを変更する際に各種データを更新するのに使用するメソッド
	 * @param animation 
	 * @param animno 
	 * @param animationdata 
	 */
	void ChangeAnimation(const aiScene *animationscene,
						 unsigned int animno,
						 tagAssimpAnimationData &animationdata);

private:
	AssimpScene m_assimpscene; // assimp scene

	// int m_Frame = 0;		   // フレーム番号
	// int m_cnt = 0;		   // キーフレーム補間用
	// int m_preFrame = 0;	   // キーフレーム補間用
	// float m_factor = 0.0f; // キーフレーム補間用

	std::map<std::string, BONE> m_Bone; //ボーンデータ（ノードの名前で参照）
	std::vector<Mesh> m_meshes;			// メッシュの集合がモデル

	std::string m_directory;				  // テクスチャファイルの位置
	ID3D11Buffer *m_constantbufferbonematrix; // ボーン行列格納用定数バッファ

	std::vector<Texture> m_texturesloaded; // 既にロードされているテクスチャ

	std::vector<Material> m_materials; // マテリアル群

	void processNode(aiNode *node, const aiScene *scene);			   // ノードを解析
	Mesh processMesh(aiMesh *mesh, const aiScene *scene, int meshidx); // メッシュを解析

	// マテリアルに対応したテクスチャを取得する
	std::vector<Texture> loadMaterialTextures(
		aiMaterial *mat,
		aiTextureType type,
		std::string typeName,
		const aiScene *scene);

	void CreateBone(aiNode *node);																	// ボーン生成
	void UpdateBoneMatrix(aiNode *node, aiMatrix4x4 matrix, std::map<std::string, BONE> &bonedata); // ボーン行列更新
	void UpdateBoneMatrixConstantBuffer(std::map<std::string, BONE> &listbone);						// ボーン行列を定数バッファに反映させる
};
