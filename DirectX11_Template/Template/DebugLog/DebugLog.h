/**
 * @file DebugLog.h
 * @author jupiter ()
 * @brief DebugLogクラスの宣言が書かれたヘッダ
 */
#include <Windows.h>
#include <string_view>
#include "../Timer/Timer.h"

#pragma once

/**
 * @brief デバッグ時にデバッグ用のテキストを表示するクラス
 * @todo 描画時に色を設定できるようにする
 */
class DebugLog
{
protected:
    /**
     * @brief 出力時に使用する標準出力
     */
    static HANDLE m_handleOutput;

    /**
     * @brief 入力時に使用する標準入力
     */
    static HANDLE m_handleInput;

    /**
     * @brief 表示の数字用
     */
    static int m_count;

    /**
     * @brief 開始してからの表示したときどれくらいたっているかの計測用
     */
    static double m_timeCounter;

    static FILE *fp;

public:
    DebugLog()
    {
        Init();
    };

    ~DebugLog()
    {
        Uninit();
    };

    void Update()
    {
        m_timeCounter += Timer::m_deltaTime;
    }

    /**
     * @brief デバッグ用の文を表示するクラス
     * @param sentence 表示させたい文
     * @n \nで改行です（入れないと改行されません）
     */
    static void Draw(std::string_view sentence);

protected:
    void Init();

    void Uninit();
};