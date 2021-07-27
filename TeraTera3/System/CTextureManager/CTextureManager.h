
#include <string_view>
#include <unordered_map>
#include <d3d11.h>

#pragma once

/**
 * @brief 読み込んだテクスチャを簡易的に管理するための構造体
 */
struct tagTextureData
{
	/**
	 * @brief SRV情報
	 */
	ID3D11ShaderResourceView *srv;

	/**
	 * @brief テクスチャの情報
	 */
	ID3D11Resource *textureData;
};

/**
 * @brief 読み込んだテクスチャを管理するクラス
 * @n ResourceContainerで管理したいがテクスチャ系のクラスはRelease解放で厳しいので保留
 */
class CTextureManager
{

	/**
	 * @brief SRV管理用のSTL
	 * @details pair型を使えばコンテナ1個になるけど長くなるので要検討
	 */
	std::unordered_map<std::string, tagTextureData *> m_pListTextureData;

	static CTextureManager *m_instance;

	CTextureManager(){};
	~CTextureManager(){};

public:
	static void Create();
	static void Delete(bool _flag = false);

	[[nodiscard]] static CTextureManager &GetInstance();

	/**
	 * @brief テクスチャを読み込む処理
	 * @param _texturename 読み込むテクスチャの名前
	 * @param _foldername テクスチャを配置しているフォルダ名 デフォルトでAssets/Textures/
	 * @return true 読み込み成功
	 * @return false 読み込み失敗 読み込み済み
	 */
	bool LoadTexture(std::string_view _texturename, std::string_view _foldername = "Assets/Textures/");

	/**
	 * @brief テクスチャを独自で読み込んだ場合にセットする方法
	 * @n フォルダ指定の方法で取得しないように注意すること！ 
	 * @n セット時にAssets/Textures/に配置してあることになります(注意！)
	 * @param _texturename セットしたいテクスチャの名前
	 * @param _srv srv情報 
	 * @param _resource 
	 * @return true 成功
	 * @return false 失敗
	 */
	bool SetTexture(std::string_view _texturename, ID3D11ShaderResourceView *_srv, ID3D11Resource *_resource);

	/**
	 * @brief 読み込んだテクスチャを取得する処理
	 * @param _texturename テクスチャの名前
	 * @param _foldername テクスチャを配置しているフォルダの名前
	 * @return tagTextureData* 取得したいテクスチャの構造体
	 */
	tagTextureData *GetTextureData(std::string_view _texturename, std::string_view _foldername = "Assets/Textures/");
};
