/**
 * @file CSceneManager.h
 * @author jupiter
 * @brief CSceneManagerが記載されたヘッダ
 */
#include <memory>

#include "../../Setup.h"
#include <unordered_map>

#pragma once

class CScene;
class ObjectGenerator;

/**
 * @brief シーンのマネージャークラス
 */
class CSceneManager
{
	/**
	 * @brief 今のシーン
	 */
	static CScene *m_pNowScene;

	/**
	 * @brief 次のシーン名
	 */
	static std::string m_nextScene;

	/**
	 * @brief シーンのリスト
	 */
	static std::unordered_map<std::string, std::unique_ptr<CScene>> *m_pListScene;

public:
	/**
	 * @brief  CSceneManagerの生成時処理
	 */
	CSceneManager();

	/**
	 * @brief CSceneManagerの破棄時処理
	 */
	~CSceneManager();

	/**
	 * @brief 毎フレームする処理
	 */
	void Update();

	/**
	 * @brief 描画処理
	 */
	void Draw();

	void ImGuiDraw(unsigned int windowid);

	/**
	 * @brief シーン変更時の処理
	 * @n つぎのUpdateの前で変更されます
	 * @return そのシーンは存在する(これから変更する)
	 */
	static bool LoadScene(std::string_view scenename);

private:
	/**
	 * @brief シーンにリストをセットする処理
	 */
	void LoadSceneList();

	/**
	 * @brief Get the Next Scenepointer
	 * @param name 変更するシーン名
	 * @return CScene* const 変更するシーンのポインタ
	 */
	[[nodiscard]] static CScene *const GetNextScene(std::string name);

	/**
	 * @brief シーンを入れ替える処理
	 */
	void ChangeScene();
};
