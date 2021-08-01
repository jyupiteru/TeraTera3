/**
 * @file ComSphere.h
 * @author jupiter ()
 * @brief ComSphereを記載したヘッダ
 */

#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <math.h>

#include "../../../Core/ComponentBase.h"
#include "../../Behavior/ComShader/ComShader.h"

/**
 * @brief 球を表示するコンポーネント
 * @todo ComBoxを参考に最適化処理を行う
 */
class ComSphere : public ComponentBase
{
private:
    /**
     * @brief 頂点バッファ
     */
    static ID3D11Buffer *m_pVertexBuffer;

    /**
     * @brief インデックスバッファ
     */
    static ID3D11Buffer *m_pIndexBuffer;

    /**
     * @brief クラスカウント用の変数
     */
    static unsigned int m_classCounter;

    /**
     * @brief 半径
     */
    float m_radius = 0.5f;

    /**
	 * @brief 頂点の情報を保存する構造体
	 */
    struct tagVertex
    {
        /**
		 * @brief 各座標
		 */
        DirectX::XMFLOAT3 Pos = {0.0f, 0.0f, 0.0f};

        /**
		 * @brief   法線
		 */
        DirectX::XMFLOAT3 Normal = {0.0f, 0.0f, 0.0f};

        /**
		 * @brief 色情報
		 */
        DirectX::XMFLOAT4 Color = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    /**
     * @brief 面の数のところで使用する構造体
     */
    struct Face
    {
        unsigned int idx[3] = {0, 0, 0};
    };

    /**
     * @brief 頂点座標等を管理する変数
     */
    tagVertex *m_vertex = nullptr;

    /**
     * @brief  面インデックスデータ
     */
    Face *m_face = nullptr;

    /**
     * @brief 面数
     */
    unsigned int m_facenum = 0;

    /**
     * @brief ComShaderへのアクセス短縮用
     */
    ComShader *m_pComShader = nullptr;

public:
    /**
     * @brief 水平方向の分割数
     * @n 大きくしすぎないように注意 100でfps4は下がったかも
     */
    CVector<unsigned int> m_divX;

    /**
     * @brief 垂直方向の分割数
     * @n 大きくしすぎないように注意 100でfps4は下がったかも
     */
    CVector<unsigned int> m_divY;

    /**
     * @brief 影の描画対象にするかを指定するフラグ 描画するならReady前にtrueにすること
     */
    bool m_flagDrawShadow = false;

public:
    virtual void Init();

    virtual void Ready();

    virtual void Uninit();

    virtual void Draw();

    virtual void ImGuiDraw(unsigned int windowid) override;

protected:
    /**
     * @brief インデックスデータを作成する
     */
    void CreateIndex();

    /**
     * @brief 頂点データを作成する
     */
    void CreateVertex();

    /**
     * @brief 色情報を更新する
     */
    void ChangeColors();

    /**
	 * @brief 正規化処理を行うメソッド
	 * @param vector 正規化したいベクトル型変数
	 * @param Normal 正規化後の変数
	 */
    void Normalize(DirectX::XMFLOAT3 vector, DirectX::XMFLOAT3 &Normal);

    /**
     * @brief 影を描画するのに使用する処理
     */
    void DrawShadow() override;
};