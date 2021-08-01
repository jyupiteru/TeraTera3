/**
 * @file CListResource.h
 * @author jupiter ()
 * @brief CListResourceの宣言、記述が記載されているヘッダ
 */

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <any>
#include "../../Setup.h"

#pragma once

/**
 * @brief 特定のリソース1種類を管理するクラス
 * @n アクセス権はCContainerクラスで管理、確認しているので無視
 */
class CListResource final
{
    /**
     * @brief リソースを保持、管理するリスト
     * @n リソース名、リソースのポインタ
     */
    std::unordered_map<std::string, std::shared_ptr<void>> m_listResource;

    /**
     * @brief 保持させるリソースデータの型
     * @n デフォルト値設定わからないのでvoid型で
     */
    std::type_index m_resourceType = typeid(void);

public:
    CListResource(){};
    virtual ~CListResource()
    {
        //存在する限り破棄
        for (auto itr = m_listResource.begin(); itr != m_listResource.end();)
        {
            itr->second.reset();
            itr = m_listResource.erase(itr);
        }
    }

    //リソースの型系

    /**
     * @brief このリストに含まれるリソースデータの型をセットする
     * @n セットしたことがない or リストの中身が0の場合変更できる
     * @tparam type セットしたいリソースデータの型情報
     * @return true 成功
     * @return false リストの中身の量が0ではない
     */
    template <class type>
    bool SetResourceType()
    {
        //リストの中身がないか確認
        if (m_listResource.size() == 0)
        {
            m_resourceType = typeid(type);
            return true;
        }
        return false;
    }

    /**
     * @brief このリストに格納されている型と同じか確認
     * @tparam type 確認したい型情報
     * @return true 同じ
     * @return false 失敗 or まだ登録されていない
     */
    template <class type>
    [[nodiscard]] bool CheckResourceType()
    {
        //同じ型か確認
        if (m_resourceType == typeid(type))
        {
            return true;
        }
        return false;
    }

    //リソースデータ系

    /**
     * @brief リソースデータを作成する処理
     * @tparam type セットしたいリソースデータの型
     * @param resourcename データに付ける名前
     * @return ポインタが返されたら作成に成功
     * @return nullptrなら既に存在する or 型情報が違う
     */
    template <class type>
    type *MakeResourceData(std::string_view resourcename)
    {
        //そのリソースはまだ登録されていないものか and 型情報がこのリストのものと一緒
        if (!CheckResourceData(resourcename.data()) && CheckResourceType<type>())
        {
            m_listResource[resourcename.data()] = std::make_shared<type>();
            return GetResourceData<type>(resourcename.data());
        }
        return nullptr;
    }

    /**
     * @brief リソースデータを登録する処理
     * @n 無理やり生ポインタを格納したものは渡すな(終わった時にエラーが出る)！
     * @tparam type セットしたいリソースデータの型
     * @param resourcename データに付ける名前
     * @param resourcedata セットしたいデータのポインタ
     * @return true セットできた
     * @return false 既に存在する or 型情報が違う
     */
    template <class type>
    bool SetResourceData(std::string_view resourcename, std::shared_ptr<type> resourcedata)
    {
        //そのリソースはまだ登録されていないものか and 型情報がこのリストのものと一緒
        if (!CheckResourceData(resourcename.data()) && CheckResourceType<type>())
        {
            m_listResource[resourcename.data()] = std::move(resourcedata);
            return true;
        }
        return false;
    }

    /**
     * @brief リソースデータを取得するメソッド
     * @tparam type 取得したいリソースの本来の型
     * @param resourcename リソース名
     * @return type const* リソースへのポインタ
     * @n nullptrなら存在しない or 型が違う
     */
    template <class type>
    [[nodiscard]] type *GetResourceData(std::string_view resourcename)
    {
        //そのリソースは登録されいる and 型情報がこのリストのものと一緒
        if (CheckResourceData(resourcename.data()) && CheckResourceType<type>())
        {
            //データを取り出しキャストして返す
            auto shareddata = m_listResource[resourcename.data()];
            auto data = std::static_pointer_cast<type>(shareddata);
            return data.get();
        }
        return nullptr;
    }

    /**
     * @brief そのリソースは存在しているか
     * @param resourcename 検索したいリソース名
     * @return true 存在する
     * @return false 存在しない
     */
    bool CheckResourceData(std::string_view resourcename)
    {
        return m_listResource.contains(resourcename.data());
    }

    /**
     * @brief 特定のリソースデータを削除するメソッド
     * @param resourcename 削除したいリソースの名前
     * @return true 削除に成功した
     * @return false 削除に失敗した
     */
    bool EraseResourceData(std::string resourcename)
    {
        //そのデータは存在するか
        if (CheckResourceData(resourcename.data()))
        {
            m_listResource.erase(resourcename.data());
        }
        return false;
    }

    //リスト系

    /**
     * @brief リストの総数を取得するメソッド
     * @return int リストの総数
     */
    int GetListTotal()
    {
        return static_cast<int>(m_listResource.size());
    }
};
