
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

    virtual void ImGui_Draw(unsigned int windowid) override;

    /**
	 * @brief 			バーテックスシェーダ生成処理
	 * @param vsfile 	shaderフォルダに入っている生成したいバーテックスシェーダのパスの入れる
     * @param flag このままキーをセットするかどうか
	 */
    void LoadVertexShader(std::string vsfile, bool flag);

    /**
	 * @brief 			バーテックスシェーダ生成処理２
	 * @param vsfile 	shaderフォルダに入っている生成したいバーテックスシェーダのパスの入れる
	 * @param layout 	使用したいlayoutを入れる
	 * @param layoutsize layoutのサイズをARRAYSIZEでとっていれる
     * @param flag このままキーをセットするかどうか
	 */
    void LoadVertexShader(std::string vsfile, D3D11_INPUT_ELEMENT_DESC *layout, unsigned int layoutsize, bool flag);

    /**
	 * @brief 			ピクセルシェーダ生成処理
	 * @param psfile 	shaderフォルダに入っている生成したいピクセルシェーダのパスを入れる
     * @param flag このままキーをセットするかどうか
	 */
    void LoadPixelShader(std::string psfile, bool flag);

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
