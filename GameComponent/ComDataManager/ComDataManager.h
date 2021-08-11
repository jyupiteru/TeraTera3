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
	static ComDataManager *m_instance;

public:
	/**
	 * @brief 生成されるステージ番号を保持する変数
	 */
	CVector<int> m_stageNum;

	/**
     * @brief すべてのマップの設計図を管理する変数
     * @n ステージ数、縦、奥、横、種類
     * @n 下、左、手前から順に描画
     */
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, E_MAPCHIP>>>> m_mapsData;

	/**
     * @brief マップに使用するオブジェクトの1辺当たりの大きさ
     */
	CVector<float> m_mapSize;

	virtual void Init() override;
	virtual void Uninit() override;
	[[nodiscard]] static ComDataManager &GetInstance();

private:
	/**
     * @brief 各マップの情報をセットする関数
     */
	void SetMap();
};
