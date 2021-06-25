/**
 * @file CDebugLog.h
 * @author jupiter ()
 * @brief CDebugLogクラスが記述されたヘッダ
 */

#include <string_view>
#include <bitset>
#include "../Setup.h"
#include "../System/CVector.h"

#pragma once

/**
 * @brief デバッグでログを表示したいときに使用するクラス
 */
class CDebugLog final
{
    /**
     * @brief 現在の状況を管理する変数
     */
    bool m_flagNowActive = true;

public:
    /**
     * @brief このクラスで出力をする際にどれでするか決めるenum
     */
    enum class E_DEBUGLOG
    {
        CONSOLE = 0,
        IMGUI,
        ALL
    };

private:
    static CDebugLog *m_instance;

    CDebugLog() { m_flagActive.SetValue(true); };
    ~CDebugLog(){};

public:
    /**
     * @brief 生存フラグ
     */
    CVector<bool> m_flagActive;

public:
    static void Create();
    static void Delete(bool _flag = false);

    /**
     * @brief 更新処理 アクティブとかの管理をここでする！
     */
    void Update();

    /**
     * @brief デバッグ時のログを表示させる際に使用する処理
     * @param _sentence 表示させたい文字列
     * @param _type どこに表示させたいか?
     * @param _color 表示させる際に使用する色
     */
    void Draw(std::string_view _sentence, E_DEBUGLOG _type = E_DEBUGLOG::CONSOLE, E_COLOR_INFO _color = E_COLOR_INFO::BLACK);

    static [[nodiscard]] CDebugLog &GetInstance();
};
