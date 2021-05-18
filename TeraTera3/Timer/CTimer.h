/**
 * @file CTimer.h
 * @author jupiter ()
 * @brief CTimerクラスを記述したヘッダ
 */
#include <chrono>
#pragma once

/**
 * @brief 時間計測クラス
 */
class CTimer
{
	/**
	 * @brief このフレームが更新されたときの時間
	 */
	static std::chrono::high_resolution_clock::time_point m_lastTime;

public:
	/**
	 * @brief 前のフレームからの経過時間が入っている
	 * @details 正確にはこのクラスのアップデートされてからの経過時間
	 */
	static double m_deltaTime;

	CTimer()
	{
		m_lastTime = std::chrono::high_resolution_clock::now();
	}

	~CTimer(){};

	/**
	 * @brief フレーム間隔を計算する
	 * @n 呼び出さなくていい
	 */
	void Update(void);

	/**
	 * @brief フレームが始まってからの経過時間を取得する
	 * @n これを2回呼ぶことで経過時間の取得が可能
	 * @return 現在のフレーム更新後の時間
	 */
	static float GetProgressTime(void);
};