/**
 * @file CConsoleLog.h
 * @author jupiter ()
 * @brief CConsoleLogクラスの宣言が書かれたヘッダ
 */
#include <Windows.h>
#include <string_view>
#include "../../Setup.h"

#pragma once

/**
 * @brief DEBUG時にデバッグ用のテキストを表示するクラス
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
     * @brief 現在生存中か判断するのに使用するフラグ
     */
    bool m_flagNowActive = true;

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
    /**
     * @brief このクラスを有効にするかどうかを管理するフラグ
     */
    CVector<bool> m_flagActive;

    friend CDebugLog;

private:

    /**
     * @brief デバッグ用の文を表示するクラス
     * @n \nで改行です（入れないと改行されません）
     * @param sentence 表示させたい文
     * @param _color 表示する際に使用する色
     */
    void Draw(std::string_view _sentence, E_COLOR_INFO _color = E_COLOR_INFO::BLACK);

    /**
     * @brief 生成処理 CDebugLogで宣言するため呼べないようにしている
     */
    static void Create();

    /**
     * @brief 破棄時処理
     */
    static void Delete();

public:
    static CConsoleLog &GetInstance();
};