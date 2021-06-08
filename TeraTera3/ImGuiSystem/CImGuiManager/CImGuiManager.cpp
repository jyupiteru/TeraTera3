/**
 * @file CImGuiManager.cpp
 * @author jupiter ()
 * @brief ImGuiManagerの実装が記載されたcpp
 */

#include "CImGuiManager.h"
#include "../CImGuiWindow/CImGuiWindow.h"
#include "CImGuiHelper/CImGuiHelper.h"
#include "../../Setup.h"

CImGuiManager *CImGuiManager::m_instance = nullptr;

void CImGuiManager::Init(HWND hWnd)
{
    // ImGuiの初期化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    m_flagSurvival.SetValue(true);

    // プラットフォームごとの描画方法を設定
    if (!ImGui_ImplWin32_Init(hWnd))
    {
        ImGui::DestroyContext();
        std::exit(1);
    }

    if (!ImGui_ImplDX11_Init(CDirectXGraphics::GetInstance().GetDXDevice(), CDirectXGraphics::GetInstance().GetImmediateContext()))
    {
        ImGui::DestroyContext();
        std::exit(1);
    }

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //スタイルのセット
    ImGui::StyleColorsDark();
    //iniを生成しないように
    io.IniFilename = NULL;

    //日本語フォントの指定
    ImFont *font = io.Fonts->AddFontFromFileTTF("Assets\\Font\\meiryo.ttc", 17.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    if (font == NULL)
    {
        exit(1);
    }

    //ヘルパー用のポインタを作成
    m_pHelper = std::make_unique<CImGuiHelper>();

    this->CreateMenu("Menu", 0);
    this->CreateMenu("CustomMenu", 1);
    this->SetImGuiFunction("Create New Window", std::bind(&CImGuiManager::ImGUi_CreateNewWindow, this, std::placeholders::_1), "Menu", E_TYPE_IMGUIFUNCTION::TRIGGER_CLICK, 0);
    this->SetImGuiFunction("Window Details", std::bind(&CImGuiManager::ImGui_Draw_WindowsDetails, this, std::placeholders::_1), "Menu", E_TYPE_IMGUIFUNCTION::DRAW, 1);
    this->SetImGuiFunction("Project Property", std::bind(&CImGuiManager::Draw_ImGui_Proparty, this, std::placeholders::_1), "Menu", E_TYPE_IMGUIFUNCTION::DRAW, 2);
}

//================================================================================================
//================================================================================================

void CImGuiManager::Uninit()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    for (auto &itr : m_listWindow)
    {
        itr.second.reset();
    }
    m_listWindow.clear();
    m_listEraseWindow.clear();

    m_listMenuName.clear();
    m_listImGuiFunction.clear();
    for (auto &itr : m_listFunctionName)
    {
        itr.second.clear();
    }
    m_listFunctionName.clear();

    m_pHelper.reset();

    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void CImGuiManager::Draw()
{
    if (m_flagSurvival.GetValue())
    {
        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

        for (auto &itr : m_listWindow)
        {
            itr.second->Draw();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        //ウインドウは１個以上存在しているか
        if (m_listWindow.size() == 0)
        { //常に１個以上生成しておく
            auto id = CreateImGuiWindow();
            auto windows = GetImGuiWindow(id);
        }
        EraseWindows();
    }
}

//================================================================================================
//================================================================================================

void CImGuiManager::Create()
{
    if (m_instance == nullptr)
    {
        m_instance = new CImGuiManager();
    }
}

//================================================================================================
//================================================================================================

void CImGuiManager::Delete(bool flag)
{
    //フラグは立っているか
    if (flag)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

//================================================================================================
//================================================================================================

bool CImGuiManager::CreateMenu(std::string_view _menuname, int _num)
{
    //_menunameは存在していなかったか?
    if (!CheckMenuName(_menuname))
    {
        int menutotalcount = static_cast<int>(m_listMenuName.size());
        m_listMenuName.emplace(_num, std::make_pair(_menuname.data(), menutotalcount));
        return true;
    }
    return false;
}

//================================================================================================
//================================================================================================

bool CImGuiManager::CheckMenuName(std::string_view _menuname)
{
    return GetMenuAccessId(_menuname).first;
}

//================================================================================================
//================================================================================================

bool CImGuiManager::SetImGuiFunction(std::string_view _functionname, std::function<void(int)> _imguifunction, std::string_view _menuname, E_TYPE_IMGUIFUNCTION _type, int _functionorder)
{
    //そのメニュー名は登録済みか?
    if (auto menudata = GetMenuAccessId(_menuname); menudata.first == true)
    {
        //その関数ポインタは登録したことがないか？
        if (auto functionAccessData = GetFunctionAccessId(menudata.second, _functionname); functionAccessData.first == false)
        {
            int functionid = static_cast<int>(m_listFunctionName[menudata.second].size());

            m_listFunctionName[menudata.second].emplace(_functionorder, std::make_pair(_functionname.data(), functionid));

            //アクセス用の特殊な文字列を生成
            std::string function_customname = "M:" + std::to_string(menudata.second);
            function_customname += "F:" + std::to_string(functionid);

            m_listImGuiFunction.emplace(function_customname, std::make_pair(_type, _imguifunction));
            return true;
        }
    }
    return false;
}

//================================================================================================
//================================================================================================

bool CImGuiManager::DestroyImGuiFunction(std::string_view _menuname, std::string_view _functionname)
{
    //メニューの情報を取得しそのメニューは存在するか判定
    if (auto menudata = GetMenuAccessId(_menuname.data()); menudata.first == true)
    {
        //関数ポインタの情報を取得しその関数ポインタは存在するか判定
        if (auto functiondata = GetFunctionAccessId(menudata.second, _functionname.data()); functiondata.first == true)
        {
            //各ウインドウに登録されていないか確認
            for (auto &itr : m_listWindow)
            {
                //対象の位置を0へ
                itr.second->m_listFlagMenu[_menuname.data()].set(functiondata.second, false);
            }

            for (auto itr = m_listFunctionName[menudata.second].begin(); itr != m_listFunctionName[menudata.second].end();)
            {
                //削除対象の関数ポインタであるか?
                if (itr->second.first == _functionname.data())
                {
                    m_listFunctionName[menudata.second].erase(itr);
                    return true;
                }
            }
        }
    }
    return false;
}

//================================================================================================
//================================================================================================

unsigned int CImGuiManager::CreateImGuiWindow()
{
    //コンストラクタをprivateにしたときにshared_ptrを作成する裏技
    struct impl : CImGuiWindow
    {
        friend CImGuiManager;
        impl(unsigned int id) : CImGuiWindow(id) {}
    };

    m_listWindow[m_windowCounter] = std::make_shared<impl>(m_windowCounter);
    m_listWindow[m_windowCounter]->Init();
    int number = m_windowCounter;
    m_windowCounter++;
    return number;
}

//================================================================================================
//================================================================================================

CImGuiWindow *CImGuiManager::GetImGuiWindow(unsigned int windowid)
{
    //引数のウインドウIDに該当するウインドウは存在するか
    if (m_listWindow.contains(windowid))
    {
        return m_listWindow[windowid].get();
    }
    return nullptr;
}

//================================================================================================
//================================================================================================

bool CImGuiManager::DestroyWindow(unsigned int windowid)
{
    //windoidのウインドウは存在しているか
    if (m_listWindow.contains(windowid))
    {
        m_listEraseWindow.push_back(windowid);
        return true;
    }
    return false;
}

//================================================================================================
//================================================================================================

void CImGuiManager::EraseWindows(void)
{
    //リストに存在する削除ウインドウをぶん回しで削除
    for (auto &itr : m_listEraseWindow)
    {
        m_listWindow[itr].reset();
        m_listWindow.erase(itr);
        m_pHelper->EraseWindowContent(itr);
    }

    m_listEraseWindow.clear();
}

//================================================================================================
//================================================================================================

void CImGuiManager::Draw_ImGui_Proparty(int)
{

    bool _flag = false;
    ImGui::Text("This is the details of the Project");
    ImGui::BulletText("Window Width : %0.0f,Height : %0.0f", static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
    ImGui::BulletText("Max Frame : %d", static_cast<int>(MAX_FRAME_RATE));
    ImGui::BulletText("Application average %.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Close AllWindows", &_flag);

    if (_flag)
    {
        m_flagSurvival.SetValue(false);
    }
}

//================================================================================================
//================================================================================================

void CImGuiManager::ImGui_Draw_WindowsDetails(int windowid)
{
    m_listWindow[windowid]->DrawWindowDetails();
}

//================================================================================================
//================================================================================================

std::pair<bool, int> CImGuiManager::GetMenuAccessId(std::string_view _menuname)
{
    //全メニューをぶん回しで検索
    for (auto &itr : m_listMenuName)
    {
        //メニュー名と一緒か
        if (itr.second.first == _menuname.data())
        {
            return {true, itr.second.second};
        }
    }

    return {false, -1};
}

//================================================================================================
//================================================================================================

std::pair<bool, int> CImGuiManager::GetFunctionAccessId(int _menuid, std::string_view _functionname)
{
    //_menuidの全登録関数をぶん回し
    for (auto &itr : m_listFunctionName[_menuid])
    {
        //_functionnameか？
        if (itr.second.first == _functionname.data())
        {
            return {true, itr.second.second};
        }
    }

    return {false, -1};
}