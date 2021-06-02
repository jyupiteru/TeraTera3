/**
 * @file CDebugLog.cpp
 * @author jupiter ()
 * @brief CDebugLogの実装が書かれたcpp
 */

#include "CDebugLog.h"
#include "CConsoleLog/CConsoleLog.h"

CDebugLog *CDebugLog::m_instance = nullptr;

void CDebugLog::Create()
{

    m_instance = new CDebugLog();
    CConsoleLog::Create();

}

//================================================================================================
//================================================================================================

void CDebugLog::Delete(bool _flag)
{
    //フラグは立っているか?
    if (_flag)
    {
        delete m_instance;
        m_instance = nullptr;

        CConsoleLog::Delete();
    }
}

//================================================================================================
//================================================================================================

void CDebugLog::Draw(std::string_view _sentence, E_DEBUGLOG _type, E_COLOR_INFO _color)
{
    if (m_flagSurvival.GetValue())
    {
        //種類によって表示先の変更
        switch (_type)
        {
        case E_DEBUGLOG::ALL: //すべてに表示させる
            //TODO ここImGuiのログ処理

        case E_DEBUGLOG::CONSOLE:
            //消えていないか?
            CConsoleLog::GetInstance().Draw(_sentence, _color);
            break;

        case E_DEBUGLOG::IMGUI:
            break;
        default:
            break;
        }
    }
}

//================================================================================================
//================================================================================================

CDebugLog &CDebugLog::GetInstance()
{
    return *m_instance;
}