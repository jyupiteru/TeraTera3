/**
 * @file ComShadow.h
 * @author jupiter ()
 * @brief ComShadowの宣言が記載されたヘッダ
 */

#include "../../../../ComSystem/ComSystem.h"

#pragma once

/**
 * @brief 影を扱うクラス
 */
class ComShadow : public ComponentBase
{
	/**
	 * @brief シングルトン用の変数
	 */
	static ComShadow *m_instance;

	/**
	 * @brief 影を描画したいオブジェクトの描画のみの関数を管理する変数
	 * @n オブジェクト名、描画関数
	 */
	std::unordered_map<std::string, std::function<void(int)>> m_listObjectDrawFunction;

public:
	ComShadow(){};
	~ComShadow(){};
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	/**
	 * @brief 影を描画する際に使用する描画のみをする関数をセットする処理
	 * @n bindを使用すること
	 * @param _function セットしたい関数
	 */
	void SetDrawShadowFuction(std::string_view _objname, std::function<void(void)> _function);

	void RemoveDrawFunction(std::string_view _objname);

private:
	void DrawShadowMap();
};
