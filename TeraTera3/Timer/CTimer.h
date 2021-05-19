/**
 * @file CTimer.cpp
 * @author jupiter ()
 * @brief Timerクラスを記述したヘッダ
 */
#include <chrono>
#include "../System/CVector.h"

#pragma once

/**
 * @brief 時間計測クラス
 */
class CTimer final
{
	/**
	 * @brief このフレームが更新されたときの時間
	 */
	std::chrono::high_resolution_clock::time_point m_lastTime;

	/**
	 * @brief シングルトン用のクラスの実体
	 */
	static CTimer *m_instance;

public:
	/**
	 * @brief 前のフレームからの経過時間が入っている
	 * @details 正確にはこのクラスのアップデートされてからの経過時間
	 */
	double m_deltaTime = 0;

private:
	CTimer();

	~CTimer();

public:
	/**
	 * @brief シングルトンを生成する関数
	 */
	static void Create();

	/**
     * @brief クラスの実体を削除する処理
     * @param flag 初期false 間違えて呼び出した時用の変数 削除時にtrueにすること
     */
	static void Delete(bool flag = false);

	/**
	 * @brief フレーム間隔を計算する
	 * @n 呼び出さないこと！
	 */
	void Update(void);

	/**
	 * @brief フレームが始まってからの経過時間を取得する
	 * @n これを2回呼ぶことで経過時間の取得が可能
	 * @return 現在のフレーム更新後の時間
	 */
	[[nodiscard]] float GetProgressTime(void);

	/**
	 * @brief このクラスの実態を取得する関数
	 * @return CTimer& 実体
	 */
	static [[nodiscard]] CTimer &GetInstance();
};