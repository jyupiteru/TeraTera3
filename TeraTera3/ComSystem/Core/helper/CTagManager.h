/**
 * @file CTagManager.h
 * @author jupiter ()
 * @brief CTagManagerの宣言が書かれたヘッダ
 */
#include <unordered_map>
#include <unordered_set>
#include <string_view>

#pragma once

class GameObject;

/**
 * @brief オブジェクトに紐づけられているタグを管理するクラス
 * @todo 細かい機能をObjectGenerator側での実装してない（まだ）なのですること！！！！
 */
class CTagManager final
{

    /**
     * @brief タグを管理するリスト
     * @n タグ名、タグに入っているオブジェクトのID
     */
    static std::unordered_map<std::string, std::unordered_set<int>> m_listObjectTag;

public:
    CTagManager(){};
    virtual ~CTagManager()
    {
        for (auto &itr : m_listObjectTag)
        {
            itr.second.clear();
        }
        m_listObjectTag.clear();
    }

    /**
     * @brief タグが存在するか確認するためのメソッド 一定時間ごとに確認して登録、破棄の処理もする
     * @param obj 確認する情報を持つオブジェクト
     * @return true 存在する
     * @return false 存在しない
     */
    bool FindObjectTag(const GameObject &obj);

    /**
     * @brief オブジェクトのタグを設定するメソッド
     * @param obj 登録するタグを持つオブジェクト
     */
    void SetObjectTag(const GameObject &obj);

    /**
     * @brief オブジェクトIDをリストから削除する処理
     * @param obj 削除したいタグを持つオブジェクト
     */
    void EraseFromTagList(const GameObject &obj);

    /**
     * @brief 特定のタグのオブジェクトIDリストを取得するメソッド
     * @param tag 取得したいリストのタグ
     * @return const std::unordered_set<int> const& タグのリスト
     */
    const std::unordered_set<int> &GetTagList(std::string_view tag)
    {
        return m_listObjectTag[tag.data()];
    }
};