/**
 * @file CImGuiWindow.h
 * @author jupiter ()
 * @brief ImGuiWindowの宣言が記載されているヘッダ
 */

#include "../../../../Thirdparty/ImGui/imgui.h"

#include <string>
#include <functional>
#include <bitset>

#pragma once

class CImGuiManager;

enum class E_FLAG_IMGUIWINDOW
{
    NO_TITLEBAR = 0,   //タイトルバーの表示
    NO_SCROLLBAR,      //スクロールバーの表示
    NO_MOVE,           //移動できるようにするか
    NO_RESIZE,         //サイズを変更できるようにするか
    NO_COLLAPSE,       //左上のたたむやつ？
    NO_NAV,            //???
    NO_BACKGROUND,     //背景を消して透明にするか
    NO_BRING_TO_FRONT, //背面にするか
    DRAW_MENUBAR,      //メニューの表示
    ALL,               //全部セットする
};

/**
 * @brief 各種の表示されるウインドウの内容クラス
 * @n 基本直接のアクセス不可
 */
class CImGuiWindow
{
protected:
    /**
     * @brief ImGui描画のために通るクラスの関数
     * @n void(*)なので注意！
     */
    std::function<void(int)> m_classFunction = nullptr;

    /**
     * @brief コンストラクタ
     * @note  自由に生成されると困るので
     */
    CImGuiWindow(unsigned int id) : m_windowId(id){};

    /**
     * @brief ウインドウの削除検知フラグ
     */
    bool m_flagSurvive = true;

    /**
     * @brief ウインドウに振られたID
     */
    int m_windowId = -1;

    /**
     * @brief 設定されている関数ポインタの名前
     */
    std::string m_functionName = "None";

    /**
     * @brief メニューから追加する各種内容を管理するリスト
     * @n メニュー名、フラグ管理用のビット
     * @details ビット操作?を使用(試験的に) 一応1つのメニューにつき8バイト確保
     */
    std::unordered_map<std::string, std::bitset<40>> m_listFlagMenu;

    /**
     * @brief 初期化時のフラグ
     */
    ImGuiWindowFlags m_flagsImGuiWindow = 0;

    std::bitset<40> m_BitflagImGuiWindow;

public:
    friend CImGuiManager;

    ~CImGuiWindow(){};

    /**
     * @brief ウインドウの名前 タイトル表示の際に使用される
     * @n デフォルトではNum:生成数
     */
    std::string m_windowName = "None";

    /**
     * @brief ウインドウの初期中心座標
     */
    ImVec2 m_firstCenterPosition = {0.0f, 0.0f};

    /**
     * @brief ウインドウの縦横の初期サイズ
     */
    ImVec2 m_firstSize = {0.0f, 0.0f};

    /**
     * @brief 現在の座標(取得のみ)
     */
    ImVec2 m_nowCenterPosition = {0.0f, 0.0f};

    /**
     * @brief 現在の大きさ(取得のみ)
     */
    ImVec2 m_nowSize = {0.0f, 0.0f};

    void Init();

    void Uninit();

    void Draw();

    //関数ポインタ系

    /**
     * @brief 特定のメニュー内容のものを表示する
     * @param _menuname 属しているメニューの名前
     * @param _functionname 登録している関数ポインタの名前
     * @param _flag 何にしたいか？ デフォルトでtrue
     */
    void SetImGuiFunction(std::string_view _menuname, std::string_view _functionname, bool _flag = true);

    /**
     * @brief ウインドウのフラグをいじる処理
     * @param _windowflag セットしたいフラグ
     * @param _flag どうしたいか
     */
    void SetWindowFlags(E_FLAG_IMGUIWINDOW _windowflag, bool _flag = true);

protected:
    /**
     * @brief メニューバーを表示する処理
     */
    void DrawMenuBar();

    /**
     * @brief ImGuiでウインドウの詳細を表示する処理
     */
    void DrawWindowDetails(void);
};
