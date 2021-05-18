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

HANDLE DebugLog::m_handleOutput;

HANDLE DebugLog::m_handleInput;

int DebugLog::m_count = 0;

double DebugLog::m_timeCounter = 0.0f;

FILE *DebugLog::fp = nullptr;

void DebugLog::Init()
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

void DebugLog::Uninit()
{
#ifdef FLAG_DEBUGLOG
    FreeConsole(); //コンソールの終了
#endif
}

void DebugLog::Draw(std::string_view sentence)
{
#ifdef FLAG_DEBUGLOG
    std::cout << "No:" << std::setw(4) << m_count;

    std::cout << " Time:" << std::setw(8) << std::fixed << std::setprecision(2) << m_timeCounter << "m";

    std::cout << " Msg:" << sentence.data() << std::endl;

    m_count++;
#endif
}