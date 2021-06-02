/**
 * @file DebugLog.cpp
 * @author jupiter ()
 * @brief DebugLogクラスの実装が書かれたcpp
 */

#include "../CDebugLog.h" //なぜかあればヘッダでの前方宣言回避できる なぜ？
#include "CConsoleLog.h"
#include <string>
#include <iostream>
#include <iomanip> // setprecisionを使用するのに必要
#include "../../Setup.h"
#include "../../Timer/CTimer.h"

CConsoleLog *CConsoleLog::m_instance = nullptr;

void CConsoleLog::Init()
{
    //コンソールの起動
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        // エクスプローラから起動した場合は新規にコンソールを割り当てる
        AllocConsole();
    }
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    m_handleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    m_handleInput = GetStdHandle(STD_INPUT_HANDLE);
}

//================================================================================================
//================================================================================================

void CConsoleLog::Uninit()
{
    FreeConsole(); //コンソールの終了

}

//================================================================================================
//================================================================================================

void CConsoleLog::Draw(std::string_view _sentence, E_COLOR_INFO _color)
{
    //TODO 色変更をできるようにしておく！

    //4桁数までのログ数を表示可能に
    std::cout << "No:" << std::setw(4) << m_count;

    //8桁数までの時間を表示可能に
    std::cout << " Time:" << std::setw(8) << std::fixed << std::setprecision(2) << CTimer::GetInstance().GetTotalTime() << "m";

    //メッセージ
    std::cout << " Msg:" << _sentence.data() << std::endl;

    m_count++;
}

//================================================================================================
//================================================================================================

void CConsoleLog::Create()
{
    //まだ生成されていないか？
    if (m_instance == nullptr)
    {
        m_instance = new CConsoleLog();
    }
}

//================================================================================================
//================================================================================================

void CConsoleLog::Delete()
{
    delete m_instance;
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

CConsoleLog &CConsoleLog::GetInstance()
{
    if (m_instance == nullptr)
    {
        Create();
    }
    return *m_instance;
}