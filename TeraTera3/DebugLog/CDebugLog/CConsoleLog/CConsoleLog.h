/**
 * @file CConsoleLog.h
 * @author jupiter ()
 * @brief CConsoleLogクラスの宣言が書かれたヘッダ
 */
#include <Windows.h>
#include <string_view>
#include "../Timer/Timer.h"

#pragma once

/**
 * @brief デバッグ時にデバッグ用のテキストを表示するクラス
 * @todo 描画時に色を設定できるようにする
 */
class CConsoleLog
{
protected:
    /**
     * @brief 出力時に使用する標準出力
     */
    HANDLE m_handleOutput;

    /**
     * @brief 入力時に使用する標準入力
     */
    HANDLE m_handleInput;

    /**
     * @brief 表示の数字用
     */
    int m_count;

    /**
     * @brief 開始してからの表示したときどれくらいたっているかの計測用
     */
    double m_timeCounter;

    /**
     * @brief 書き込み先のハンドル(多分アドレス)
     */
    FILE *fp;

    static CConsoleLog *m_instance;

    CConsoleLog()
    {
        Init();
    };

    ~CConsoleLog()
    {
        Uninit();
    };

    void Init();

    void Uninit();

public:
    void Update()
    {
        m_timeCounter += Timer::m_deltaTime;
    }

    /**
     * @brief デバッグ用の文を表示するクラス
     * @param sentence 表示させたい文
     * @n \nで改行です（入れないと改行されません）
     */
    void Draw(std::string_view sentence);

    static void Create();

    static void Delete(bool flag = false);

    CConsoleLog &GetInstance();
};