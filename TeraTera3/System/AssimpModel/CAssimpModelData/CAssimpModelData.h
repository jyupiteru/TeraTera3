/**
 * @file CAssimpModelData.h
 * @author jupiter ()
 * @brief CAssimpModelDataを宣言しているヘッダ
 */

#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <assimp\scene.h>
#include <map>
#include <assimp\Importer.hpp>

#include "CAssimpScene/CAssimpScene.h"
#include "Cmesh/CMesh.h"
#include "../CAssimpAnimationData/CAssimpAnimationData.h"
#include "../../CDirectXGraphics/CDirectXGraphics.h"

class Com3DModelAssimp;

/**
 * @brief アニメーションの際に必要になる各種データ
 */
struct tagAssimpAnimationData
{
    /**
     * @brief フレーム番号
     */
    int m_frame = 0;

    /**
     * @brief アニメーションの最大フレーム数
     */
    int m_maxflame = 0;

    /**
     * @brief ???
     */
    float m_TickPerSecond;

    /**
     * @brief キーフレーム補間用
     */
    int m_preFrame = 0;

    /**
     * @brief キーフレーム補間用
     */
    int m_cnt = 0;

    /**
     * @brief キーフレーム補間用
     */
    float m_factor = 0.0f;

    /**
     * @brief ボーンデータ（ノードの名前で参照）
     */
    std::map<std::string, tagBONE> m_listBone;
};

/**
 * @brief Assimpで読み込んだモデルを表示したりするクラス
 */
class CAssimoModelData
{
    /**
     * @brief 読み込んだ変換済みモデルの情報
     */
    CAssimpScene m_assimpscene;

    /**
     * @brief ノードの名前で検索可能なボーンデータ
     */
    std::map<std::string, tagBONE> m_Bone;

    /**
     * @brief モデルの各メッシュ
     */
    std::vector<CMesh> m_meshes;

    /**
     * @brief テクスチャファイルの位置
     */
    std::string m_directory;

    /**
     * @brief ボーン行列格納用定数バッファ
     */
    ID3D11Buffer *m_constantbufferbonematrix;

    /**
     * @brief 既にロードされているテクスチャ
     */
    std::vector<tagTexture> m_texturesloaded;

    /**
     * @brief マテリアル群
     */
    std::vector<tagMaterial> m_materials;

public:
    friend Com3DModelAssimp;

    CAssimoModelData();
    ~CAssimoModelData();

    void Uninit();

    /**
     * @brief アニメーションの更新処理
     * @param _mtxworld 行列
     * @param _animationscene アニメーションシーン
     * @param _animno アニメーション番号
     * @param _animationdata アニメーションのデータ
     */
    void UpdateAnimation(const DirectX::XMFLOAT4X4 &_mtxworld, const aiScene *_animationscene, unsigned int _animno, tagAssimpAnimationData &_animationdata);

    /**
     * @brief 描画処理
     * @param _mtxworld オブジェクトの行列
     * @param _animationdata アニメーションの情報
     * @param _color オブジェクトの色情報
     */
    void Draw(DirectX::XMFLOAT4X4 &_mtxworld, tagAssimpAnimationData &_animationdata, DirectX::XMFLOAT4 _color);

    /**
     * @brief モデルを読み込む処理
     * @param _resourcefolder テクスチャの存在するフォルダ
     * @param _filename ファイルの名前
     * @return true 成功
     * @return false 失敗
     */
    bool Load(std::string _resourcefolder, std::string _filename);

    /**
     * @brief メッシュを取得する処理
     * @return const std::vector<Mesh>& 取得したいメッシュ情報
     */
    const std::vector<CMesh> &GetMeshes() const;

    /**
     * @brief マテリアルを読み込む処理
     */
    void LoadMaterial();

    /**
	 * @brief アニメーションを変更する際に各種データを更新するのに使用する処理
	 * @param _animation アニメーションのデータ
	 * @param _animno アニメーションの番号
	 * @param _animationdata 現在のフレーム数などを格納した構造体
	 */
    void ChangeAnimation(const aiScene *_animationscene, unsigned int _animno, tagAssimpAnimationData &_animationdata);

private:
    /**
     * @brief ノードを解析する処理
     * @param _node 解析したいノードの情報
     * @param _scene ??
     */
    void ProcessNode(aiNode *_node, const aiScene *_scene);

    /**
     * @brief 目種を解析する処理
     * @param mesh
     * @param scene
     * @param meshidx
     * @return Mesh 解析したメッシュ
     */
    CMesh ProcessMesh(aiMesh *_mesh, const aiScene *_scene, int _meshidx);

    /**
     * @brief マテリアルに対応したテクスチャを取得する処理
     * @param _mat
     * @param _type
     * @param _typeName
     * @param _scene
     * @return std::vector<Texture>
     */
    std::vector<tagTexture> LoadMaterialTextures(aiMaterial *_mat, aiTextureType _type, std::string _typeName, const aiScene *_scene);

    /**
     * @brief ボーンを生成する処理
     * @param node 生成元？
     */
    void CreateBone(aiNode *node);

    /**
     * @brief ボーン行列更新
     * @param _node
     * @param _matrix 座標
     * @param _bonedata
     */
    void UpdateBoneMatrix(aiNode *_node, aiMatrix4x4 _matrix, std::map<std::string, tagBONE> &_bonedata);

    /**
     * @brief ボーン行列を定数バッファに反映させる処理
     * @param _listbone
     */
    void UpdateBoneMatrixConstantBuffer(std::map<std::string, tagBONE> &_listbone, DirectX::XMFLOAT4 _color);
};
