/**
 * @file CImGuiHelper.cpp
 * @author jupiter ()
 * @brief CImGuiHelperクラスの実装が記載されたcpp
 */

#include "CImGuiHelper.h"

std::unordered_map<unsigned int, std::unordered_map<std::string, std::string>> CImGuiHelper::m_listWindowDisplayContent;

std::unordered_map<unsigned int, std::unordered_map<std::string, std::u8string>> CImGuiHelper::m_listWindowDisplayContent_UTF8;

void CImGuiHelper::Uninit()
{
    //普通のほうのリストの削除処理
    for (auto &itr : m_listWindowDisplayContent)
    {
        itr.second.clear();
    }

    //utf6型のほうの削除処理
    for (auto &itr : m_listWindowDisplayContent_UTF8)
    {
        itr.second.clear();
    }
}

void CImGuiHelper::EraseWindowContent(unsigned int windowid)
{
    //そのウインドウIDのものは存在するか
    if (m_listWindowDisplayContent.contains(windowid))
    {
        m_listWindowDisplayContent.erase(windowid);
    }

    //そのウインドウIDのものは存在するか
    if (m_listWindowDisplayContent_UTF8.contains(windowid))
    {
        m_listWindowDisplayContent_UTF8.erase(windowid);
    }
}

void CImGuiHelper::SetWindowDisplayContent(unsigned int windowid, std::string_view functionname, std::string_view contentname, std::string_view details)
{
    //関数名?と変数名?から検索用のタグを生成
    std::string tag = functionname.data();
    tag += "__";
    tag += contentname.data();

    m_listWindowDisplayContent[windowid][tag] = details.data();
}

//================================================================================================
//================================================================================================

void CImGuiHelper::SetWindowDisplayContent_UTF8(unsigned int windowid, std::string_view functionname, std::string_view contentname, std::u8string_view details)
{
    //関数名?と変数名?から検索用のタグを生成
    std::string tag = functionname.data();
    tag += "__";
    tag += contentname.data();

    m_listWindowDisplayContent_UTF8[windowid][tag] = details.data();
}

//================================================================================================
//================================================================================================

std::string CImGuiHelper::GetWindowDisplayContent(unsigned int windowid, std::string_view functionname, std::string_view contentname)
{
    //関数名?と変数名?から検索用のタグを生成
    std::string tag = functionname.data();
    tag += "__";
    tag += contentname.data();

    //ウインドウIDのものは登録されたことあるか
    if (m_listWindowDisplayContent.contains(windowid))
    {
        //検索用タグのものは登録されたことあるか
        if (m_listWindowDisplayContent[windowid].contains(tag))
        {
            return m_listWindowDisplayContent[windowid][tag];
        }
    }

    //存在しないのでNoneを返す
    return "None";
}

//================================================================================================
//================================================================================================

std::u8string CImGuiHelper::GetWindowDisplayContent_UTF8(unsigned int windowid, std::string_view functionname, std::string_view contentname)
{

    //関数名?と変数名?から検索用のタグを生成
    std::string tag = functionname.data();
    tag += "__";
    tag += contentname.data();

    //ウインドウIDのものは登録されたことあるか
    if (m_listWindowDisplayContent_UTF8.contains(windowid))
    {
        //検索用タグのものは登録されたことあるか
        if (m_listWindowDisplayContent_UTF8[windowid].contains(tag))
        {
            return m_listWindowDisplayContent_UTF8[windowid][tag];
        }
    }

    //存在しないのでNoneを返す
    return u8"None";
}
