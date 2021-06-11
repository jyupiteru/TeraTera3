/**
 * @file ComMapManager.h
 * @author jupiter ()
 * @brief ComMapManagerクラスの宣言が記述されているヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief マップチップの種類の列挙型
 */
enum class E_MAPCHIP
{
	FLOOR,
	NONE,
};

/**
 * @brief マップを管理するクラス
 * @n 生成も行う
 */
class ComMapManager : public ComponentBase
{

	/**
	 * @brief マップ情報を保持する変数
	 * @n ステージ数、縦、横、内容
	 */
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, E_MAPCHIP>>> m_listMapData;

	/**
	 * @brief マップに配置するオブジェクトを管理するリスト
	 */
	std::vector<GameObject *> m_listMapObject;

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

	/**
	 * @brief マップを生成する処理
	 * @param _stagenum 何個目のマップを生成するか?
	 */
	void CreateMap(int _stagenum);

	/**
	 * @brief すべてのマップをゲームオーバーの時の状態にする
	 * @n ComGameOverからのみ呼び出すこと
	 */
	void SetMapGameOver();

private:
	/**
	 * @brief マップ用のオブジェクトを生成する処理
	 * @param _objname オブジェクトに設定する名前
	 * @return GameObject* 生成したオブジェクトのポインタ
	 */
	GameObject *MakeMapObj(std::string_view _objname);
};
