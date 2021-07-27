/**
 * @file CImGuiHelper.h
 * @author jupiter ()
 * @brief CImGuiクラスがきさされているヘッダ
 */

#pragma once

#include <unordered_map>

/**
 * @brief ImGUiを使用する各ウインドウの関数の変更できる変更内容の現在の内容を管理できるクラス
 * @details ContainerクラスとCListResourceクラスで代用できるが処理を分けたい、ほかに処理を追加したいので分離
 */
class CImGuiHelper
{
    /**
     * @brief ウインドウの表示内容を管理する変数1
     * @n ウインドウID、関数名+__+その変数の前？、内容
     */
    static std::unordered_map<unsigned int, std::unordered_map<std::string, std::string>> m_listWindowDisplayContent;

    /**
     * @brief ウインドウの表示内容を管理する変数2
     * @n ウインドウID、関数名+__+その変数の前？、内容(日本語はこっち)
     */
    static std::unordered_map<unsigned int, std::unordered_map<std::string, std::u8string>> m_listWindowDisplayContent_UTF8;

    void Uninit();

public:
    CImGuiHelper(){};
    ~CImGuiHelper() { Uninit(); };

    /**
     * @brief ウインドウが削除された際に使用する関連のもの削除する処理
     * @n これは削除時に自動で読み込まれる
     * @param windowid ウインドウに振られたID
     */
    void EraseWindowContent(unsigned int windowid);

    /**
     * @brief ウインドウに描画する際に必要になる内容の保存する処理1
     * @n utf-8型はもう１個のほうを使用
     * @param windowid ウインドウのID
     * @param functionname 関数ポインタ名(重複しそうにないもの)
     * @param contentname 変数名
     * @param details 表示する内容
     */
    static void SetWindowDisplayContent(unsigned int windowid, std::string_view functionname, std::string_view contentname, std::string_view details);

    /**
     * @brief ウインドウに描画する際に必要になる内容の保存する処理2
     * @n utf-8型はもう１個のほうを使用
     * @param windowid ウインドウのID
     * @param functionname 関数ポインタ名(重複しそうにないもの)
     * @param contentname 変数名
     * @param details utf-8型の表示する内容
     */
    static void SetWindowDisplayContent_UTF8(unsigned int windowid, std::string_view functionname, std::string_view contentname, std::u8string_view details);

    /**
     * @brief 保存していたImGuiで表示する内容を取得する処理1
     * @n utf-8型はもう１つのものを使用
     * @param windowid ウインドウのID
     * @param functionname 登録する際に使用した関数ポインタ名
     * @param contentname 登録する際に使用した変数名
     * @return std::string 登録していた内容
     * @n "None"なら未登録なので注意
     */
    [[nodiscard]] static std::string GetWindowDisplayContent(unsigned int windowid, std::string_view functionname, std::string_view contentname);

    /**
     * @brief 保存していたImGuiで表示する内容を取得する処理2
     * @param windowid ウインドウのID
     * @param functionname 登録する際に使用した関数ポインタ名
     * @param contentname 登録する際に使用した変数名
     * @return std::string utf-8型の登録していた内容
     * @n "None"なら未登録なので注意
     */
    [[nodiscard]] static std::u8string GetWindowDisplayContent_UTF8(unsigned int windowid, std::string_view functionname, std::string_view contentname);
};
