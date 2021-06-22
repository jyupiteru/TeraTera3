
/**
 * @file ComShader.h
 * @author jupiter
 * @brief ComShaderクラスが記述されたヘッダ
 * @version 1.0
 * @date 2020-08-07
 */

#include <unordered_map>

#include "../../../Core/ComponentBase.h"
#include "../../../Core/GameObject.h"

#include "../../../../WindowsSystem/Shader/Shader.h"
#include "../../../../ResourceContainer/CContainer.h"

#pragma once

/**
 * @brief シェーダ処理を管理するコンポーネントクラス
 * @todo まだリソースコンテナに追加できていないのですること
 */
class ComShader final : public ComponentBase
{
private:
    /**
	 * @brief バーテックスシェーダー管理用のSTL
	 */
    static std::unordered_map<std::string, ID3D11VertexShader *> m_pVertexShaders;

    /**
	 * @brief ピクセルシェーダーの管理用のSTL
	 */
    static std::unordered_map<std::string, ID3D11PixelShader *> m_pPixelShaders;

    /**
	 * @brief layout管理用のSTL キーはバーテックスと一緒
	 */
    static std::unordered_map<std::string, ID3D11InputLayout *> m_pLayout;

    /**
	 * @brief クラスの総生成数
	 */
    static int m_classCount;

public:
    /**
	 * @brief バーテックスシェーダ、layoutを連想配列より引き出すときのキー
	 */
    std::string m_keyVertexShader;

    /**
	 * @brief ピクセルシェーダ用のキー
	 */
    std::string m_keyPixelShader;

    /**
	 * @brief  CShaderの生成時処理
	 */
    ComShader()
    {
        m_classCount++;
    };

    /**
	 * @brief CShaderの破棄時処理
	 */
    ~ComShader(){};

public:
    /**
	 * @brief デフォルトのシェーダ読み込み処理
	 */
    virtual void Init() override;

    /**
	 * @brief 連想配列削除処理 このクラスを使用しているクラスが0で処理が通る
	 */
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
	 * @param psfile 	Shaderフォルダに入っている生成したいピクセルシェーダのパスを入れる
     * @param flag このままキーをセットするかどうか デフォルトはtrue
     * @param _Foldername 読み込むシェーダーを格納しているフォルダ名 デフォルトはDefaultPixel
	 */
    void LoadPixelShader(std::string psfile, bool _flag = true, std::string_view _foldername = "DefaultPixel");

    /**
     * @brief 頂点シェーダーを変更する処理 LoadVertexShaderはレイアウト等がいるので追加しました。
     * @param _shadername 変更したい読み込み済みシェーダーの名前
     * @return true 成功
     * @return false 失敗 まだ読み込めていない
     */
    bool ChangeVertexShader(std::string_view _shadername);

    //Set系
    /**
	 * @brief バーテックスシェーダをセットする処理
	 */
    void SetVertexShader()
    {
        SetVertexShader(m_keyVertexShader);
    }

    /**
     * @brief 指定して頂点シェーダをセットする処理
     * @param key セットしたい頂点シェーダ
     */
    void SetVertexShader(std::string key);

    /**
	 * @brief ピクセルシェーダをセットする処理
	 */
    void SetPixelShader()
    {
        SetPixelShader(m_keyPixelShader);
    }

    void SetPixelShader(std::string key);

    //Get系
    /**
	 * @brief 				バーテックスシェーダとlayoutを他のオブジェクトに渡す処理
	 * @details				MakeVertexShaderで受け取る
	 * @return std::string 	キーを渡す
	 */
    [[nodiscard]] std::string GetVertexShader()
    {
        return m_keyVertexShader;
    }

    /**
	 * @brief 				ピクセルシェーダを他のオブジェクトに渡す処理
	 * @details				MakePixekShaderで受け取る
	 * @return std::string 	ピクセルシェーダのキー
	 */
    [[nodiscard]] std::string GetPixelShader()
    {
        return m_keyPixelShader;
    }
};
