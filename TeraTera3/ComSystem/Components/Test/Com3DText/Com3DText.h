/**
 * @file Com3DText.h
 * @author jupiter ()
 * @brief Com3DTextクラスの宣言が書かれたヘッダ
 */

#pragma once

#include "../../../Core/Cores.h"
#include "../../Behavior/ComShader/ComShader.h"
#include "../../Behavior/Com3DTexture/Com3DTexture.h"

/**
 * @brief 文字を表示するためのコンポーネント
 * @todo 未完成
 */
class Com3DText : public Com3DTexture
{
    std::vector<int> m_objectList;

    ComShader *m_pShader = nullptr;

    std::string m_fontKey;

    //static std::unordered_map<std::string, > m_fontList;

public:
    /**
     * @brief 表示したい文字列(現在英数字のみ)
     */
    std::string m_text;

    /**
     * @brief Uiとして表示するか3Dオブジェクトとして画像で表示するか決める変数
     * @n OBJECT3Dで3Dオブジェクト
     */
    E_TYPE_COMPONENT type = E_TYPE_COMPONENT::OBJECT2D;

    /**
	 * @brief  Component U I Textureの生成時処理
	 */
    Com3DText(){};

    /**
	 * @brief Component U I Textureの破棄時処理
	 */
    ~Com3DText(){};

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Update() override;

    virtual void Draw() override;

    /**
     * @brief フォントの画像を新しくセットする場合に使用
     * @param name Asset下のFontフォルダに入れたフォント画像の名前を入れる
     * @param issetall すべてに適応するか
     */
    void SetNewFontTexture(std::string_view name, bool issetall);

private:
    void UpdateVertex();

    // 頂点構造体
    struct Vertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT4 Color;
        DirectX::XMFLOAT2 Tex;
    };

    // インデックス構造体
    struct Face
    {
        unsigned int idx[3];
    };
};
