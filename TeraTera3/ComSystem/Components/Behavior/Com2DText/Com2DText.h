/**
 * @file Com2DText.h
 * @author jupiter ()
 * @brief Com2DTextの宣言が書かれたヘッダ
 */
#pragma once

#include "../../../Core/ComponentBase.h"
#include "../../Behavior/ComShader/ComShader.h"

//表示できる文字れつの最大数
#define MAXSTRINGSIZE 200

/**
 * @brief 英数字を2Dで表示させるコンポーネントクラス
 * @todo このコンポーネントから3Dのテキストクラスを親として作成する
 */
class Com2DText : public ComponentBase
{
    typedef struct
    {
        DirectX::XMFLOAT4 viewPortWidth;  // ビューポート幅
        DirectX::XMFLOAT4 viewPortHeight; // ビューポート高さ
    } tagConstantBufferViewPort;

    /**
     * @brief スクリーン情報
     */
    static tagConstantBufferViewPort m_screenData; // b5

protected:
    // 頂点構造体
    struct tagVertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT4 Color;
        DirectX::XMFLOAT2 Tex;
    };

    // インデックス構造体
    struct tagFace
    {
        unsigned int idx[3];
    };

    /**
	 * @brief このクラス生成数を管理するためのメンバ
	 */
    static int m_classCount;

    /**
	 * @brief 頂点バッファ
	 */
    ID3D11Buffer *m_vertexBuffer = nullptr;

    /**
	 * @brief インデックスバッファ
	 */
    ID3D11Buffer *m_idxBuffer = nullptr;

    /**
	 * @brief 定数バッファ
	 */
    ID3D11Buffer *m_cbuffer = nullptr;

    /**
	 * @brief ???
	 * @details シェーダにスクリーンの大きさを渡すのに使用
	 */
    ID3D11Buffer *m_screenBuffer = nullptr;

    // 頂点データ
    std::vector<tagVertex> m_listVertex;

    // インデックスデータ
    std::vector<tagFace> m_listIndex;

    /**
	 * @brief SRV管理用のSTL
	 */
    static std::unordered_map<std::string, ID3D11ShaderResourceView *> m_pListSRV;

    /**
	 * @brief フォントのテクスチャ管理用のSTL
	 */
    static std::unordered_map<std::string, ID3D11Resource *> m_pListFontTexture;

    /**
	 * @brief フォントのテクスチャ、SRV管理用のキー
	 */
    std::string m_keyFontTexture;

    /**
     * @brief ComShaderへの簡易アクセス用
     */
    ComShader *m_pShader = nullptr;

public:
    /**
     * @brief 描画したい文字列
     */
    std::string m_text = "no text";

    /**
     * @brief 文字描画時に縦横の比率を合わせるかのフラグ
     * @n 横（最終的な）に合わせます（縦は無視）
     */
    CVector<bool> m_flagSetRate;

    Com2DText(){};
    ~Com2DText(){};

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Ready() override;

    virtual void Update() override;

    virtual void Draw() override;

    virtual void ImGui_Draw(unsigned int windowid) override;

    /**
     * @brief 新しくフォントを読み込む
     * @param fontname 変えたいフォントの画像名(Assets下のFontフォルダに入れる)
     */
    virtual void LoadFontTexture(std::string_view fontname);

    /**
     * @brief フォントの検索用のキーを取得する
     * @return std::string 使用しているフォントのパス
     */
    std::string GetFontKey(void)
    {
        return m_keyFontTexture;
    }

protected:
    /**
     * @brief 描画する文字列等を初期化するメソッド
     */
    void ClearString(void);

    /**
     * @brief VertexとIndexのバッファ更新用
     */
    void UpdateBuffer();

    /**
     * @brief 頂点バッファとインデックスバッファを生成、更新するメソッド
     */
    void CreateVertexIndexData();

    /**
     * @brief 引数の文字フォントの位置を取得するメソッド
     * @param chara 取得したい文字（英数字）
     * @param fontuv この文字のフォント位置
     */
    void GetFontUV(char chara, DirectX::XMFLOAT2 *fontuv);
};
