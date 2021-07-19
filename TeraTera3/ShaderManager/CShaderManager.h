/**
 * @file CShaderManager.h
 * @author jupiter ()
 * @brief CShaderManagerクラスの宣言が記載されているヘッダ
 */

#pragma once

/**
 * @brief シェーダーを管理するクラス
 * @n コンポーネント以外でもシェーダーを使用することがあったのでComShaderから分離
 */
class CShaderManager
{
	static CShaderManager *m_instance;

public:
	static void Create();
	static void Destroy(bool _flag = false);
	static CShaderManager &GetInstance();
};
