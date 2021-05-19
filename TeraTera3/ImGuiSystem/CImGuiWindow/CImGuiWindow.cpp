/**
 * @file CImGuiWindow.cpp
 * @author jupiter ()
 * @brief CImGuiWindowの実装が記載されたcpp
 */

#include "CImGuiWindow.h"
#include "../CImGuiManager/CImGuiManager.h"
#include "../../Setup.h"
#include "../CImGuiManager/CImGuiHelper/CImGuiHelper.h"

#include <bitset>
#include <bit>

void CImGuiWindow::Init()
{
    m_windowName = "Num:" + std::to_string(m_windowId);

    //ここで共通処理として実装し細かく変更したかったら後から
    SetWindowFlags(E_FLAG_IMGUIWINDOW::ALL, false);
    //SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_TITLEBAR, true);
    SetWindowFlags(E_FLAG_IMGUIWINDOW::DRAW_MENUBAR, true);
    SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_COLLAPSE, true);
    SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_NAV, true);
    SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_SCROLLBAR, true);

    //初期座標等の設定
    m_firstCenterPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    m_firstSize = {SCREEN_WIDTH / 3, SCREEN_WIDTH / 2};
}

//================================================================================================
//================================================================================================

void CImGuiWindow::Uninit()
{
    m_listFlagMenu.clear();
}

//================================================================================================
//================================================================================================

void CImGuiWindow::Draw()
{

    //座標、大きさともにAppearingの設定で出現時のみ適応に設定
    ImGui::SetNextWindowPos(ImVec2(m_firstCenterPosition.x - m_firstSize.x / 2, m_firstCenterPosition.y - m_firstSize.y / 2), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(m_firstSize, ImGuiCond_Appearing);

    m_flagsImGuiWindow = static_cast<int>(m_BitflagImGuiWindow.to_ulong());

    //ウインドウの描画
    if (!ImGui::Begin(m_windowName.c_str(), &m_flagSurvive, m_flagsImGuiWindow))
    {
        ImGui::End();
        return;
    }

    //メニューバーの表示
    DrawMenuBar();

    //大きさの更新
    m_nowSize.y = ImGui::GetWindowHeight();
    m_nowSize.x = ImGui::GetWindowWidth();

    //左上の座標を取得
    m_nowCenterPosition = ImGui::GetWindowPos();

    m_nowCenterPosition.x += m_nowSize.x / 2.0f;
    m_nowCenterPosition.y += m_nowSize.y / 2.0f;

    auto list_menu = &CImGuiManager::GetInstance().m_listMenuName;
    auto list_func = &CImGuiManager::GetInstance().m_listImGuiFunction;

    //各メニューのぶん回し
    for (auto &itr : *list_menu)
    {
        //このメニューの登録されている関数ポインタを取得する
        auto list_menufunc = &CImGuiManager::GetInstance().m_listFunctionName[itr.second.second];
        for (auto &itr2 : *list_menufunc)
        {
            //この関数ポインタのフラグは立っているか?
            if (m_listFlagMenu[itr.second.first].test(itr2.second.second))
            {
                //アクセス用の特殊な文字列を生成
                std::string function_customname = "M:" + std::to_string(itr.second.second);
                function_customname += "F:" + std::to_string(itr2.second.second);

                //この関数はどの処理を行うものか？
                switch ((*list_func)[function_customname].first)
                {

                case E_TYPE_IMGUIFUNCTION::DRAW:

                    //表示と選択されたか?
                    if (ImGui::TreeNode(itr2.second.first.c_str()))
                    {
                        (*list_func)[function_customname].second(m_windowId);
                        ImGui::TreePop();
                    }
                    break;

                case E_TYPE_IMGUIFUNCTION::TRIGGER_CLICK:
                    //クリック時のみなので元に戻す
                    m_listFlagMenu[itr.second.first].set(itr2.second.second, false);
                    [[fallthrough]]; //フォールスルー警告解除の属性

                case E_TYPE_IMGUIFUNCTION::CLICK:
                    //TRIGGERとほぼ同じ処理なので通る
                    (*list_func)[function_customname].second(m_windowId);
                    break;
                }
            }
        }
    }

    ImGui::End();

    //このウインドウは削除されたか
    if (!m_flagSurvive)
    {
        CImGuiManager::GetInstance().DestroyWindow(m_windowId);
    }
}

//================================================================================================
//================================================================================================

void CImGuiWindow::SetImGuiFunction(std::string_view _menuname, std::string_view _functionname, bool _flag)
{
    auto menu_accessdata = CImGuiManager::GetInstance().GetMenuAccessId(_menuname.data());
    auto function_accessdata = CImGuiManager::GetInstance().GetFunctionAccessId(menu_accessdata.first, _functionname.data());

    //メニューと対象の関数ポインタは存在したのか?
    if (function_accessdata.first)
    {
        //その関数ポインタの位置にフラグをセットする
        m_listFlagMenu[_menuname.data()].set(function_accessdata.second, _flag);
    }
}

//================================================================================================
//================================================================================================

void CImGuiWindow::SetWindowFlags(E_FLAG_IMGUIWINDOW _windowflag, bool _flag)
{
    unsigned int flagnum = 0;
    switch (_windowflag)
    {
    case E_FLAG_IMGUIWINDOW::NO_TITLEBAR:
        flagnum = ImGuiWindowFlags_NoTitleBar;
        break;

    case E_FLAG_IMGUIWINDOW::NO_SCROLLBAR:
        flagnum = ImGuiWindowFlags_NoScrollbar;
        break;

    case E_FLAG_IMGUIWINDOW::DRAW_MENUBAR:
        flagnum = ImGuiWindowFlags_MenuBar;
        break;

    case E_FLAG_IMGUIWINDOW::NO_MOVE:
        flagnum = ImGuiWindowFlags_NoMove;
        break;

    case E_FLAG_IMGUIWINDOW::NO_RESIZE:
        flagnum = ImGuiWindowFlags_NoResize;
        break;

    case E_FLAG_IMGUIWINDOW::NO_COLLAPSE:
        flagnum = ImGuiWindowFlags_NoCollapse;
        break;

    case E_FLAG_IMGUIWINDOW::NO_NAV:
        flagnum = ImGuiWindowFlags_NoNav;
        break;

    case E_FLAG_IMGUIWINDOW::NO_BACKGROUND:
        flagnum = ImGuiWindowFlags_NoBackground;
        break;

    case E_FLAG_IMGUIWINDOW::NO_BRING_TO_FRONT:
        flagnum = ImGuiWindowFlags_NoBringToFrontOnFocus;
        break;

    case E_FLAG_IMGUIWINDOW::ALL:
        //再帰処理?で全部フラグセット
        SetWindowFlags(E_FLAG_IMGUIWINDOW::DRAW_MENUBAR, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_BACKGROUND, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_BRING_TO_FRONT, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_COLLAPSE, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_MOVE, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_NAV, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_RESIZE, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_SCROLLBAR, _flag);
        SetWindowFlags(E_FLAG_IMGUIWINDOW::NO_TITLEBAR, _flag);
        return;
        break;
    }

    //表現するのに何ビット必要か取得
    unsigned int bitnum = std::bit_width(flagnum);

    m_BitflagImGuiWindow.set(bitnum - 1, _flag);
}

//================================================================================================
//================================================================================================

void CImGuiWindow::DrawMenuBar()
{
    bool flag_newwindow = false;

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    //メニューバーの開始
    if (ImGui::BeginMenuBar())
    {

        auto list_menufunc = &CImGuiManager::GetInstance().m_listFunctionName;
        auto list_menu = &CImGuiManager::GetInstance().m_listMenuName;

        for (auto &itr : *list_menu)
        {
            //各種メニューを表示し選択されたか？
            if (ImGui::BeginMenu(itr.second.first.c_str()))
            {
                //このメニュー内で表示する一連の関数ポインタを表示
                auto list_menufunc = &CImGuiManager::GetInstance().m_listFunctionName[itr.second.second];

                //表示する関数ポインタをぶん回し
                for (auto &itr2 : *list_menufunc)
                {
                    //引数の場所はセットされているか？
                    bool flag_details = m_listFlagMenu[itr.second.first].test(itr2.second.second);

                    ImGui::MenuItem(itr2.second.first.c_str(), NULL, &flag_details);

                    //添え時の場所に直接アクセスしビットをいれる set、resetでも可能(if分岐いる)
                    m_listFlagMenu[itr.second.first].set(itr2.second.second, flag_details);
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();
    }
}

//================================================================================================
//================================================================================================

void CImGuiWindow::DrawWindowDetails(void)
{
    ImGui::BulletText("Center Position");
    ImGui::Indent();
    ImGui::Text("X : %0.1f,Y : %0.1f", m_nowCenterPosition.x, m_nowCenterPosition.y);
    ImGui::Unindent();

    ImGui::BulletText("Size");
    ImGui::Indent();
    ImGui::Text("X : %0.1f,Y : %0.1f", m_nowSize.x, m_nowSize.y);
    ImGui::Unindent();
}
