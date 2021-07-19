/**
 * @file CShadowManager.h
 * @author jupiter ()
 * @brief CShadowManagerクラスの宣言が記載されたヘッダ
 */

#pragma once

/**
 * @brief 影を表示するのに必要なシャドウマップを生成するクラス
 */
class CShadowManager
{

	static CShadowManager *m_instance;

public:
	CShadowManager();
	~CShadowManager();

	static void Create(){};
	static void Destroy(bool _flag = false);
	static CShadowManager &GetInstance();

	void SetDrawFunction();

	void RemoveDrawFunction();
};
