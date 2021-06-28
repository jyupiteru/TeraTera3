/**
 * @file ComWipeManager.h
 * @author jupiter ()
 * @brief ComWipeManagerの宣言が記載されているヘッダ
 */

#include <unordered_map>
#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief 現在のワイプを管理するときに使用する列挙体
 */
enum class E_TYPE_WIPE
{
	GAME_WIPE_CLOSE,
	GAME_WIPE_OPEN,
	RESULT_WIPE_CLOSE,
	RESULT_WIPE_OPEN,
	END
};

/**
 * @brief ワイプの挙動を管理するコンポーネント
 */
class ComWipeManager : public ComponentBase
{
	static ComWipeManager *m_instance;

	/**
	 * @brief 現在動作しているワイプの種類
	 */
	E_TYPE_WIPE m_typeWipe = E_TYPE_WIPE::END;

	/**
	 * @brief ワイプを管理しているコンポーネントへの簡易アクセス用変数
	 */
	ComWipe *m_comWipe = nullptr;

	/**
	 * @brief ワイプの動作スピード
	 */
	float m_wipeSpeed = 400.0f;

	/**
	 * @brief 現在のワイプの大きさ
	 */
	float m_sizeCount = 0.0f;

	/**
	 * @brief 時間のカウント用変数
	 */
	float m_timeCounter = 0.0f;

	/**
	 * @brief ワイプが閉じて開けるまでの時間
	 */
	float m_waitTime = 2.0f;

public:
	ComWipeManager(){};
	~ComWipeManager(){};
	void Init() override;
	void Uninit() override;
	void Ready() override;
	void Update() override;
	static ComWipeManager &GetInstance();

	/**
	 * @brief ワイプをスタートさせる処理
	 * @param _type 
	 */
	void StartWipe(E_TYPE_WIPE _type);

	/**
	 * @brief 終了したかどうかを確認する処理
	 * @return true 終了した
	 * @return false 終了していない
	 */
	bool IsEnd();

	/**
	 * @brief 現在のワイプの種類を取得する処理
	 * @return E_TYPE_WIPE 現在のワイプの種類
	 */
	E_TYPE_WIPE GetNowWipeType();

private:
	/**
	 * @brief 閉じるワイプを更新する処理
	 */
	void UpdateWipeClose();

	/**
	 * @brief 開けるワイプの更新処理
	 */
	void UpdateWipeOpen();
};
