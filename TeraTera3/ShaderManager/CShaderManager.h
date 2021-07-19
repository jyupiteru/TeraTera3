/**
 * @file CShaderManager.h
 * @author jupiter ()
 * @brief CShaderManagerクラスの宣言が記載されているヘッダ
 */

#include <unordered_map>
#include <string>
#include <d3d11.h>

#pragma once

/**
 * @brief シェーダーを管理するクラス
 * @n コンポーネント以外でもシェーダーを使用することがあったのでComShaderから分離
 */
class CShaderManager
{
	static CShaderManager *m_instance;

	/**
	 * @brief バーテックスシェーダー管理用のSTL
	 */
	std::unordered_map<std::string, ID3D11VertexShader *> m_pVertexShaders;

	/**
	 * @brief ピクセルシェーダーの管理用のSTL
	 */
	std::unordered_map<std::string, ID3D11PixelShader *> m_pPixelShaders;

	/**
	 * @brief layout管理用のSTL キーはバーテックスと一緒
	 */
	std::unordered_map<std::string, ID3D11InputLayout *> m_pLayout;

public:
	static void Create();
	static void Delete(bool _flag = false);
	static CShaderManager &GetInstance();

	/**
	 * @brief 頂点シェーダ生成処理
	 * @param _vsfile Shaderフォルダに入っている生成したい頂点シェーダのパスの入れる
	 * @param _layout 使用したいlayoutを入れる
	 * @param _layoutsize layoutのサイズをARRAYSIZEでとっていれる
     * @param _Foldername 読み込むシェーダーを格納しているフォルダ名 デフォルトはDefaultVertex
	 */
	void LoadVertexShader(std::string_view _vsfile, D3D11_INPUT_ELEMENT_DESC *_layout, unsigned int _layoutsize, std::string_view _foldername = "DefaultVertex");

	/**
	 * @brief ピクセルシェーダ生成処理
	 * @param psfile Shaderフォルダに入っている生成したいピクセルシェーダのパスを入れる
     * @param _Foldername 読み込むシェーダーを格納しているフォルダ名 デフォルトはDefaultPixel
	 */
	void LoadPixelShader(std::string_view psfile, std::string_view _foldername = "DefaultPixel");

	//Set系

	/**
     * @brief 指定して頂点シェーダをセットする処理
     * @param key セットしたい頂点シェーダの名前
     */
	void SetVertexShader(std::string key);

	/**
	 * @brief キー以外の特定のシェーダーをセットする処理
	 * @param key セットしたいピクセルシェーダーの名前
	 */
	void SetPixelShader(std::string key);

	/**
	 * @brief 頂点シェーダの情報を取得する処理
	 * @param _shadername 頂点シェーダの名前
	 * @param _foldername 格納しているフォルダの名前 デフォルトはDefaultVertex
	 * @return ID3D11VertexShader* 頂点シェーダ情報
	 */
	[[nodiscard]] ID3D11VertexShader *GetVertexShaderData(std::string_view _shadername, std::string_view _foldername = "DefaultVertex");

	/**
	 * @brief 頂点レイアウトを直接取得する処理
	 * @param _shadername ピクセルシェーダの名前
	 * @param _foldername 格納しているフォルダの名前 デフォルトはDefaultPixel
	 * @return ID3D11InputLayout* 頂点レイアウトの情報
	 */
	[[nodiscard]] ID3D11InputLayout *GetLayoutData(std::string_view _shadername, std::string_view _foldername = "DefaultPixel");

	/**
	 * @brief ピクセルシェーダの情報を取得する処理
	 * @param _shadername 頂点シェーダの名前
	 * @param _foldername 格納しているフォルダの名前 デフォルトはDefaultVertex
	 * @return ID3D11PixelShader* ピクセルシェーダの情報
	 */

	[[nodiscard]] ID3D11PixelShader *GetPixelShaderData(std::string_view _shadername, std::string_view _foldername = "DefaultVertex");

	/**
	 * @brief デバッグウインドウで各種シェーダーを表示する処理
	 * @param unsigned int ウインドウのID
	 */
	void ImGuiDraw(unsigned int);
};
