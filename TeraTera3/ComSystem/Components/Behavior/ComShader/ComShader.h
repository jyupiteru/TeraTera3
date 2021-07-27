
/**
 * @file ComShader.h
 * @author jupiter
 * @brief ComShaderクラスが記述されたヘッダ
 */

#include <unordered_map>

#include "../../../Core/ComponentBase.h"
#include "../../../Core/GameObject.h"

#include "../../../../System/Shader/Shader.h"
#include "../../../../Managers/ResourceContainer/CContainer.h"

#pragma once

/**
 * @brief シェーダー関連の処理をおこなうコンポーネントクラス
 * @n シェーダーを管理しているのはCShaderManagerクラス
 */
class ComShader final : public ComponentBase
{

    /**
	 * @brief バーテックスシェーダ、layoutを連想配列より引き出すときのキー
	 */
    std::string m_keyVertexShader;

    /**
	 * @brief ピクセルシェーダ用のキー
	 */
    std::string m_keyPixelShader;

public:
    /**
	 * @brief  CShaderの生成時処理
	 */
    ComShader(){};

    /**
	 * @brief CShaderの破棄時処理
	 */
    ~ComShader(){};

public:
    virtual void Init() override;
    virtual void Uninit() override;
    virtual void ImGuiDraw(unsigned int windowid) override;

    /**
	 * @brief 			頂点シェーダ生成処理
	 * @param _vsfile 	Shaderフォルダに入っている生成したい頂点シェーダのパスの入れる
	 * @param _layout 	使用したいlayoutを入れる
	 * @param _layoutsize layoutのサイズをARRAYSIZEでとっていれる
     * @param _flag このままキーをセットするかどうか デフォルトはtrue
     * @param _Foldername 読み込むシェーダーを格納しているフォルダ名 デフォルトはDefaultVertex
	 */
    void LoadVertexShader(std::string _vsfile, D3D11_INPUT_ELEMENT_DESC *_layout, unsigned int _layoutsize, bool _flag = true, std::string_view _foldername = "DefaultVertex");

    /**
	 * @brief 			ピクセルシェーダ生成処理
	 * @param _psfile 	Shaderフォルダに入っている生成したいピクセルシェーダのパスを入れる
     * @param _flag このままキーをセットするかどうか デフォルトはtrue
     * @param _Foldername 読み込むシェーダーを格納しているフォルダ名 デフォルトはDefaultPixel
	 */
    void LoadPixelShader(std::string _psfile, bool _flag = true, std::string_view _foldername = "DefaultPixel");

    /**
     * @brief 頂点シェーダーを変更する処理 LoadVertexShaderはレイアウト等がいるので追加しました。
     * @param _shadername 変更したい読み込み済みシェーダーの名前
     * @param _foldername フォルダの名前 デフォルトはDefaultVertex
     * @return true 成功
     * @return false 失敗 まだ読み込めていない
     */
    bool ChangeVertexShader(std::string_view _shadername, std::string_view _foldername = "DefaultVertex");

    //Set系
    /**
	 * @brief キーを使用してバーテックスシェーダをセットする処理
	 */
    void SetVertexShader();

    /**
     * @brief 指定して頂点シェーダをセットする処理
     * @param key セットしたい頂点シェーダ
     */
    void SetVertexShader(std::string key);

    /**
	 * @brief キーを使用してピクセルシェーダをセットする処理
	 */
    void SetPixelShader();

    /**
     * @brief 指定してピクセルシェーダーをセットする処理
     * @param key セットしたいピクセルシェーダ
     */
    void SetPixelShader(std::string key);
};
