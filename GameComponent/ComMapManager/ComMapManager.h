
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


	static ComMapManager *m_instance;

public:
	/**
	 * @brief 縦横どれくらい生成するか
	 * @n 横、縦 初期は10,10
	 */
	CVector2<int> m_mapMax;

	/**
	 * @brief ステージの半分の大きさ
	 */
	CVector<float> m_MaphalfSize;

	ComMapManager(){};
	~ComMapManager(){};

	void Init() override;

	void Uninit() override;

	void Ready() override;

	void Update() override;

	void Draw() override;

	static [[nodiscard]] ComMapManager &GetInstance();

	void CreateMap(int _stagenum);

private:
	GameObject *MakeMapObj(std::string_view _objname);
};
