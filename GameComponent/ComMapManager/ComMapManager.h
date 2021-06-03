
#include "../../TeraTera3/TeraTera.h"

#pragma once

enum class E_MAPCHIP
{
	FLOOR,
	NONE,
};

class ComMapManager : public ComponentBase
{

	/**
	 * @brief マップ情報を保持する変数
	 * @n ステージ数、縦、横、内容
	 */
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, E_MAPCHIP>>> m_listMapData;

	/**
	 * @brief 各ステージの横、奥行どれくらいが最大か保持するリスト
	 * @n ステージ数、億、横
	 */
	std::unordered_map<int, std::pair<int, int>> m_listMapMaxNum;

	static ComMapManager *m_instance;

	int m_stageWidthMax = 20;

	int m_stageDepthMax = 20;

public:
	ComMapManager(){};
	~ComMapManager(){};

	void Init() override;

	void Uninit() override;

	void Ready() override;

	void Update() override;

	void Draw() override;

	static [[nodiscard]] ComMapManager &GetInstance();

	void CreateMap(int _stagenum);
};
