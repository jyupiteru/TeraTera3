/**
 * @file CTimer.cpp
 * @author jupiter ()
 * @brief Timerクラスを記述したヘッダ
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
	double m_deltaTime;

	CTimer()
	{
		Init();
	}

	~CTimer()
	{
		Uninit();
	}

	void Init();

	void Uninit();

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
	[[nodiscard]] float GetProgressTime(void);

	static [[nodiscard]] CTimer &GetInstance();
};