/**
 * @file ComDataManager.h
 * @author jupiter ()
 * @brief ComDataManagerクラスの宣言のあるヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#include <unordered_map>

#pragma once

/**
 * @brief ステージ作成時に使用する列挙型
 */
enum class E_MAPCHIP
{
	NONE = 0,
	START,
	GOAL,
	FLOOR,
	FLOOR_FALL,

};

/**
 * @brief ゲームを通して保持する必要のある情報を保持するコンポーネント
 */
class ComDataManager : public ComponentBase
{
	struct tagLoadStageData
	{
		/**
		 * @brief マップの大きさ
		 */
		float m_stageChipSize;

		/**
		 * @brief このマップの耐久値の最大値
		 */
		float m_hitPointMax;

		float m_hitPointMin;

		float m_decreaseValue;

		std::unordered_map<int, std::unordered_map<int, E_MAPCHIP>> m_stageData;
	};

	static ComDataManager *m_instance;

public:
	/**
	 * @brief 生成されるステージ番号を保持する変数
	 */
	CVector<int> m_stageNum;

	/**
     * @brief すべてのステージの設計図を管理する変数
     * @n ステージ数、奥、横、種類
     * @n 下、左、手前から順に描画
	 * @n 0はチュートリアル用ステージのため注意
     */
	std::unordered_map<int, tagLoadStageData *> m_stagesData;

	/**
     * @brief ステージに使用するオブジェクトの1辺当たりの大きさ
     */
	CVector<float> m_mapSize;

	virtual void Init() override;
	virtual void Uninit() override;
	[[nodiscard]] static ComDataManager &GetInstance();

private:
	/**
	 * @brief ステージを読み込んでくる処理
	 * @n map〇と書かれているモノを数字順にあるものすべて読み込みしている
	 */
	void LoadStage();
};
