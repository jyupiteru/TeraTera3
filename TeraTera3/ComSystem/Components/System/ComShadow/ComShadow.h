/**
 * @file ComShadow.h
 * @author jupiter ()
 * @brief ComShadowの宣言が記載されたヘッダ
 */

#include "../../../../ComSystem/ComSystem.h"
#include "../../Behavior/ComShader/ComShader.h"
#include "../ComLight/ComLight.h"

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
	std::unordered_map<std::string, std::function<void(void)>> m_listObjectDrawFunction;

	/**
	 * @brief 各種コンポーネントへの簡易アクセス用変数
	 */
	ComShader *m_comShader = nullptr;
	ComLight *m_comLight = nullptr;

public:
	ComShadow(){};
	~ComShadow(){};
	void Init() override;
	void Uninit() override;
	void Ready() override;
	void Update() override;
	void Draw() override;

	/**
	 * @brief 影を描画する際に使用する描画のみをする関数をセットする処理
	 * @n bindを使用すること
	 * @param _function セットしたい関数
	 */
	void SetDrawShadowFuction(std::string_view _objname, std::function<void(void)> _function);

	/**
	 * @brief 関数を削除する処理
	 * @param _objname 削除対象のオブジェクト
	 */
	void RemoveDrawFunction(std::string_view _objname);

private:
	/**
	 * @brief シャドウマップを描画する処理
	 */
	void DrawShadowMap();
};
