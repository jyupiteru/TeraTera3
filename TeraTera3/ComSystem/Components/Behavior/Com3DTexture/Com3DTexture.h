/**
 * @file Com3DTexture.h
 * @author jupiter
 * @brief Textureコンポーネントが記述されたヘッダ
 * @date 2020-08-21
 */

#include <d3d11.h>
#include <unordered_map>
#include <memory>
#include <DirectXMath.h>

#include "../../../Core/ComponentBase.h"
#include "../ComShader/ComShader.h"

#define VERTEX_2D_NUM 4

#pragma once

//!@brief	板ポリゴンの生成方向を決める際に使用
//!@details	オブジェクトの当たり判定にも使用
enum class E_TYPE_TEXTUREOBJ
{
	HORIZON = 0, //!<地面と平行に表示	大きさはx,z使用 yの＋に向いて描写
	VERTICAL,	 //!<z軸と直角に生成	大きさはx,y使用 zの-向いて描写
	DEPTH		 //!<x軸と直角に生成 	大きさはy,z使用 xの+向いて描写
};

/**
 * @brief シェーダに渡す頂点情報等
 */
struct tagVertex
{
	float x = 0.0f, y = 0.0f, z = 0.0f;		//座標
	DirectX::XMFLOAT4 color = {0, 0, 0, 0}; //色
	float tu = 0.0f, tv = 0.0f;				//テクスチャ座標
};

/**
 * @brief 3D空間に画像を表示するコンポーネント
 */
class Com3DTexture : public ComponentBase
{
protected:
	ComShader *m_pShader = nullptr;

	/**
	 * @brief テクスチャ表示方向
	 */
	E_TYPE_TEXTUREOBJ m_typeTexture = E_TYPE_TEXTUREOBJ::DEPTH;

	/**
	 * @brief SRV管理用のSTL
	 * @details pair型を使えばコンテナ1個になるけど長くなるので要検討
	 */
	static std::unordered_map<std::string, ID3D11ShaderResourceView *> m_pListSRV;

	/**
	 * @brief テクスチャ管理用のSTL
	 */
	static std::unordered_map<std::string, ID3D11Resource *> m_pListTexture;

	/**
	 * @brief テクスチャ、SRV管理用のキー
	 */
	std::string m_keyTexture;

	/**
	 * @brief このクラス生成数を管理するためのメンバ
	 */
	static int m_classCount;

	/**
	 * @brief 頂点バッファ
	 */
	ID3D11Buffer *m_vertexbuffer = nullptr;

	/**
	 * @brief インデックスバッファ
	 */
	ID3D11Buffer *m_idxbuffer = nullptr;

	/**
	 * @brief 定数バッファ
	 */
	ID3D11Buffer *m_cbuffer = nullptr;

	/**
	 * @brief ???
	 * @details シェーダにスクリーンの大きさを渡すのに使用
	 */
	ID3D11Buffer *m_screenBuffer = nullptr;

	/**
	 * @brief 画像の大きさやUV座標を保持するメンバ
	 */
	tagVertex m_vertex[4];

	//!@brief		現在の何個目のテクスチャか 左上を1,1とする
	DirectX::XMFLOAT2 m_textureNum = {1, 1};

	//!@brief		テクスチャ１枚当たりの割合
	DirectX::XMFLOAT2 m_textureRate = {1.0f, 1.0f};

public:
	Com3DTexture(){};
	virtual ~Com3DTexture(){};

	virtual void Init() override;

	virtual void Uninit() override;

	virtual void Ready() override;

	virtual void Draw() override;

	virtual void ImGuiDraw(unsigned int windowid) override;

	/**
	 * @brief 画像読み込み処理
	 * @param texturename 読み込みたい画像名(assets/textureファイルの中に入っている物)
	 * @param texturetype E_TYPE_TEXTUREOBJ型 オブジェクト生成方向を指定
	 */
	void LoadTexture(std::string texturename, E_TYPE_TEXTUREOBJ texturetype);

	//!@brief				テクスチャの場所変更
	//!@param	num			左上が1,1として指定
	void ChangeTextureNum(DirectX::XMFLOAT2 num)
	{
		m_textureNum = {(num.x), (num.y)};
		SetUV();
	}

	//!@brief				テクスチャの一枚当たりの割合指定 ％で
	//!@param	num			指定したい割合を入れる
	void SetTextureRate(DirectX::XMFLOAT2 num)
	{
		m_textureRate = num;
		SetUV();
	}

	//!@brief				テクスチャを何分割するか指定　何枚に分けるみたいな
	//!@param	num			何枚に分けたいか入れる
	void SetTextureRateNumber(DirectX::XMFLOAT2 num)
	{
		m_textureRate = {(float)(1.0f / num.x), (float)(1.0f / num.y)};
		SetUV();
	}

	/**
	 * @brief テクスチャ検索用キーの取得
	 * @return std::string 取得したいキー
	 */
	[[nodiscard]] std::string GetTextureData()
	{
		return m_keyTexture;
	}

protected:
	/**
	 * @brief 画像の色等いろいろセットする関数
	 */
	virtual void SetVertex();

	/**
	 * @brief 画像のUV座標をセットする関数
	 */
	void SetUV();
};
