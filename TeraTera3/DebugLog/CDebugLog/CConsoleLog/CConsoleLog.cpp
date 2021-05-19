/**
 * @file DebugLog.cpp
 * @author jupiter ()
 * @brief DebugLogクラスの実装が書かれたcpp
 */
#include "DebugLog.h"
#include <string>
#include <iostream>
#include <iomanip> // setprecisionを使用するのに必要
#include "../Setup.h"

CConsoleLog *CConsoleLog::m_instance = nullptr;

void CConsoleLog::Init()
{
#ifdef FLAG_DEBUGLOG
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
#endif
}

//================================================================================================
//================================================================================================

void CConsoleLog::Uninit()
{
#ifdef FLAG_DEBUGLOG
    FreeConsole(); //コンソールの終了
#endif
}

//================================================================================================
//================================================================================================

void CConsoleLog::Draw(std::string_view sentence)
{
#ifdef FLAG_DEBUGLOG
    //4桁数までのログ数を表示可能に
    std::cout << "No:" << std::setw(4) << m_count;

    //8桁数までの時間を表示可能に
    std::cout << " Time:" << std::setw(8) << std::fixed << std::setprecision(2) << m_timeCounter << "m";

    //メッセージ
    std::cout << " Msg:" << sentence.data() << std::endl;

    m_count++;
#endif
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

void CConsoleLog::Delete(bool flag = false)
{
    //フラグは立っているか？
    if (flag)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

//================================================================================================
//================================================================================================

CConsoleLo &CConsoleLog::GetInstance()
{
    if (m_instance == nullptr)
    {
        Create();
    }
    return &m_instance;
}