#pragma once

#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include <map>
#include "../../AssimpVertexsData.h"

// テクスチャ構造体
struct tagTexture
{
    std::string type;                  // テクスチャ種類
    std::string path;                  // テクスチャパス名
    ID3D11Resource *resource;          // リソース
    ID3D11ShaderResourceView *texture; // SRV
};

/**
 * @brief Assimpで使用するモデルのメッシュ情報を管理するクラス
 */
class CMesh
{

    /**
     * @brief 頂点バッファ
     */
    ID3D11Buffer *m_vertexBuffer;

    /**
     * @brief インデックスバッファ
     */
    ID3D11Buffer *m_indexBuffer;

    /**
     * @brief 定数バッファ
     */
    ID3D11Buffer *m_cbmtrl;

    /**
     * @brief マテリアル
     */
    tagMaterial m_mtrl;

public:
    /**
     * @brief 頂点データ
     */
    std::vector<tagAssimpVertex> m_vertices;

    /**
     * @brief インデックス
     */
    std::vector<unsigned int> m_indices;

    /**
     * @brief テクスチャ情報
     */
    std::vector<tagTexture> m_textures;

    //ID3D11Device *m_dev;                 // デバイス

    /**
     * @brief ボーン毎の頂点データ
     */
    std::unordered_map<std::string, std::vector<DirectX::XMFLOAT3>> m_listBoneVertices;

    /**
     * @brief Construct a new CMesh object
     * @param vertices 頂点情報
     * @param indices インデックス
     * @param textures テクスチャ
     * @param mtrl マテリアル
     */
    CMesh(std::vector<tagAssimpVertex> _vertices, std::vector<unsigned int> _indices, std::vector<tagTexture> _textures, tagMaterial _mtrl);

    /**
     * @brief Construct a new CMesh object
     * @param vertices 頂点情報
     * @param indices インデックス
     * @param textures テクスチャ
     */
    CMesh(std::vector<tagAssimpVertex> _vertices, std::vector<unsigned int> _indices, std::vector<tagTexture> _textures);

    /**
     * @brief 終了処理
     */
    void Uninit();

    /**
     * @brief 描画処理
     */
    void Draw();

private:
    /**
     * @brief 各種バッファの生成処理？
     * @return true 成功
     * @return false 失敗
     */
    bool CreateBuffer();

    /**
     * @brief 各種バッファの更新処理
     */
    void UpdateVertexBuffer();
};