/**
 * @file SceneSetter.h
 * @author jupiter ()
 * @brief SceneSetterの宣言、実装が書かれたヘッダ
 */

#include <memory>
#include <unordered_map>

#include "../TeraTera3/SceneSystem/CScene.h"

#include "CSceneSample/CSceneSample.h"
#include "CSceneSample2/CSceneSample2.h"
#include "CSceneTitle/CSceneTitle.h"
#include "CSceneGame1/CSceneGame1.h"
#include "CSceneResult/CSceneResult.h"

#pragma once

/**
 * @brief ゲームで読み込むシーンを設定するクラス
 */
class SceneSetter
{
	/**
	 * @brief シーンの一覧を格納するポインタ
	 */

	std::shared_ptr<std::unordered_map<std::string, std::unique_ptr<CScene>>> m_pListScene = nullptr;

public:
	~SceneSetter(){};

	/**
	* @brief 一番初めに読み込むシーン
	**/
	std::string firstSceneName = "SceneGame1";

	/**
	 * @brief Get the Set Scene object
	 * @return std::unordered_map<std::string, std::shared_ptr<CScene>>* シーン一覧を格納したリストのポインタを渡す
	 */
	std::shared_ptr<std::unordered_map<std::string, std::unique_ptr<CScene>>> GetSetScene()
	{
		m_pListScene = std::make_shared<std::unordered_map<std::string, std::unique_ptr<CScene>>>();
		//引数に名前をいれる
		(*m_pListScene)["SceneSample"] = std::make_unique<CSceneSample>();
		(*m_pListScene)["SceneSample2"] = std::make_unique<CSceneSample2>();
		(*m_pListScene)["SceneTitle"] = std::make_unique<CSceneTitle>();
		(*m_pListScene)["SceneGame1"] = std::make_unique<CSceneGame1>();
		(*m_pListScene)["SceneResult"] = std::make_unique<CSceneResult>();

		return m_pListScene;
	}
};
