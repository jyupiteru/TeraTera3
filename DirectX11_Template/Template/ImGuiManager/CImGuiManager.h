/**
 * @file CImGuiManager.h
 * @author jupiter ()
 * @brief CImGuiManagerの宣言が記載されたヘッダ
 */

#include "../../Library/ImGui/imgui.h"
#include "../../Library/ImGui/imgui_impl_dx11.h"
#include "../../Library/ImGui/imgui_impl_win32.h"

#include "ImGuiHelperFunctions.h"
#include "../Systems/CDirectXGraphics/CDirectXGraphics.h"
#include "CImGuiHelper.h"

#include <string>
#include <map>
#include <unordered_map>
#include <functional>

class CImGuiWindow;

#pragma once

/**
 * @brief ImGuiのメニュー選択時の各種処理を決めれる列挙型
 */
enum class E_TYPE_IMGUIFUNCTION
{
    /**
         * @brief クリックのみの処理
         */
    CLICK = 0,

    /**
         * @brief 表示の処理
         */
    DRAW,

    /**
         * @brief クリックした時のみ通る
         */
    TRIGGER_CLICK,
};

/**
 * @brief ImGuiの表示を管理するマネージャー
 */
class CImGuiManager
{
    /**
     * @brief メニューの順番を管理するリスト
     * @n メニューの順番、(メニュー名、string重複なんか嫌なのでアクセス用変数(生成数を使用))
     */
    static std::multimap<int, std::pair<std::string, int>> m_listMenuName;

    /**
     * @brief ImGuiの表示に使用するメニュー名とその中で表示させる関数ポインタアクセス名を管理するリスト
     * @n メニューアクセス変数、関数の出力順(デフォルト50)、(関数ポインタ名、関数アクセス用のid<==これをビット操作に使用する)
     */
    static std::unordered_map<int, std::multimap<int, std::pair<std::string, int>>> m_listFunctionName;

    /**
     * @brief 関数ポインタを管理するリスト すべて一括管理
     * @n M:+メニューアクセス変数+F:+関数ポインタ管理名、関数ポインタ
     */
    static std::unordered_map<std::string, std::pair<E_TYPE_IMGUIFUNCTION, std::function<void(int)>>> m_listImGuiFunction;

    /**
     * @brief ウインドウを管理しているリスト
     * @n ウインドウid、その詳細
     */
    static std::map<unsigned int, std::shared_ptr<CImGuiWindow>> m_listWindow;

    /**
     * @brief ウインドウを削除する際に使用するリスト
     */
    static std::vector<unsigned int> m_listEraseWindow;

    /**
     * @brief ウインドウの総生成数を管理する変数
     */
    static int m_windowCounter;

    /**
     * @brief 関数表示時のヘルパー用のクラスへのポインタ
     * @n 基本子のポインタからは使用しないです
     */
    static std::unique_ptr<CImGuiHelper> m_pHelper;

    /**
     * @brief ImGuiの破棄処理
     */
    void Uninit();

public:
    friend CImGuiWindow;

    CImGuiManager(){};
    ~CImGuiManager() { Uninit(); };

    /**
     * @brief ImGuiの初期化処理
     */
    void Init(HWND hWnd);

    void Draw();

    //メニュー系

    /**
     * @brief メニュー作成用処理
     * @param _menuname 生成するメニュー名
     * @param _num メニューの表示順
     * @return true 成功
     * @return false 既に存在している
     */
    static bool CreateMenu(std::string_view _menuname, int _num);

    /**
     * @brief メニュー名が存在するか確認する処理
     * @param _menuname 確認したいメニュー名
     * @return true 成功
     * @return false 既に存在している
     */
    static bool CheckMenuName(std::string_view _menuname);

    //関数ポインタ系

    /**
      * @brief クラスに存在するvoid ImGuiDraw(int)いう関数を登録する処理
      * @n std::functionが生成できそうならそれを渡してもう１個のメソッドを使用すること
      * @param _functionname 選択する際に使用する名前
      * @tparam classtype 登録したい関数が存在するクラスのポインタ
      * @param _menuname 表示する際に使用するメニュー名 デフォルト値 custom
      * @param _type クリック時に行われる処理の種類
      * @param _functionorder ImGuiの順番 デフォルト50
      * @return true 成功
      * @return false 失敗
      */
    template <Has_ImGUiDraw classtype>
    static bool SetImGuiFunction(std::string_view _functionname, classtype *classpointer, std::string_view _menuname = "CustomMenu", E_TYPE_IMGUIFUNCTION _type = E_TYPE_IMGUIFUNCTION::DRAW, int _functionorder = 50)
    {
        return SetImGuiFunction(_functionname, std::bind(&classtype::ImGuiDraw, classpointer, std::placeholders::_1), _menuname, _type, _functionorder);
    }

    /**
     * @brief 特定のImGuiを表示する関数ポインタをセットする関数
     * @param _functionname 選択する際に使用する名前
     * @param _imguifunction void(int)のImGuiの表示する関数ポインタ
     * @param _menuname 表示する際に使用するメニュー名 デフォルト値 custom
     * @param _type クリック時に行われる処理の種類
     * @param _functionorder ImGuiの順番 デフォルト50
     * @return true 成功
     * @return false 失敗
     */
    static bool SetImGuiFunction(std::string_view _functionname, std::function<void(int)> _imguifunction, std::string_view _menuname = "CustomMenu", E_TYPE_IMGUIFUNCTION _type = E_TYPE_IMGUIFUNCTION::DRAW, int _functionorder = 50);

    /**
     * @brief ImGuiの関数ポインタを削除うする処理する処理
     * @n 元が消えた際に消すのに使用
     * @param _menuname 関数ポインタが属しているメニュー名
     * @param functionname 消す関数ポインタに着けた名前
     * @return true 成功
     * @return false 失敗or存在しない
     */
    static bool DestroyImGuiFunction(std::string_view _menuname, std::string_view _functionname);

    //ウインドウの作成系

    /**
     * @brief ImGUiのウインドウを新しく生成する処理
     * @return unsigned int ウインドウに振られるID 忘れるとアクセスはほぼ不可能
     */
    static unsigned int CreateImGuiWindow();

    /**
     * @brief ImGuiのウインドウの表示を行っているクラスを取得する処理
     * @n CImguiWindow.hもincludeすること
     * @param windowid ウインドウに振られているID
     * @return CImGuiWindow* ウインドウへのポインタ
     * @details 生成時のみのアクセスを想定
     */
    [[nodiscard]] static CImGuiWindow *GetImGuiWindow(unsigned int windowid);

    /**
     * @brief ウインドウを破棄する処理
     * @n 基本使わないと思われる処理
     * @param windowid 各ウインドウに振られているID
     * @return true 成功した
     * @return false 失敗した
     */
    static bool DestroyWindow(unsigned int windowid);

protected:
    /**
     * @brief ウインドウを一括で削除する処理
     */
    void EraseWindows(void);

    void ImGUi_CreateNewWindow(int)
    {
        CreateImGuiWindow();
    }

    /**
     * @brief プロジェクトのプロパティをImGuiのウインドウに表示する際に使用する処理
     * @param none 引数特になし
     */
    void Draw_ImGui_Proparty(int);

    void ImGui_Draw_WindowsDetails(int windowid);

    /**
     * @brief メニューのアクセスidを取得する関数
     * @param _menuname 取得したい対象のメニュー名
     * @return std::pair<bool, int> boolがtrueなら成功でintにidがfalseなら失敗でidに-1
     */
    static std::pair<bool, int> GetMenuAccessId(std::string_view _menuname);

    /**
     * @brief 関数のアクセスidを取得する関数
     * @param _menuid 取得したい対象の属しているメニューのid
     * @param _functionname 取得したい関数ポインタの名前
     * @return std::pair<bool, int> boolがtrueなら成功でintにidがfalseなら失敗でidに-1
     */
    static std::pair<bool, int> GetFunctionAccessId(int _menuid, std::string_view _functionname);
};
