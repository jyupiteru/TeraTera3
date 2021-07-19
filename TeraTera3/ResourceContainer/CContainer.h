/**
 * @file CContainer.h
 * @author jupiter ()
 * @brief CContainerクラスの宣言を記述したヘッダ
 */

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include "../Setup.h"
#include "CListResource.h"

#pragma once

/**
 * @brief 各種リソースを管理するクラス
 * @n リソースの削除はゲーム終了時に行われます
 */
class CContainer final
{
    /**
     * @brief 各種リソースのリスト、ポインタを管理するためのリスト
     * @n 取得したいリスト名(重複不可)、(管理する際に付ける名前、汎用ポインタ的な)
     */
    std::unordered_map<std::string, std::shared_ptr<CListResource>> m_listResources;

    /**
     * @brief リストに安全にアクセスさせる際に使用するリスト
     * @n リストの名前、リストにアクセスできるクラスの型ID
     */
    std::unordered_map<std::string, int> m_listAccessData;

    /**
     * @brief リストの型とそのIDを管理するリスト
     * @n 型情報、型ID
     * @details 引数の２個目にコンストラクタがないものを使用できずこうなった
     */
    std::unordered_map<std::type_index, int> m_listClassType;

    static CContainer *m_instance;

    CContainer(){};
    ~CContainer()
    {
        m_listAccessData.clear();
        m_listClassType.clear();
        m_listResources.clear();
    }

public:
    static void Create();

    static void Delete(bool _flag = false);

    static CContainer &GetInstance();

    /**
     * @brief クラスのコンテナへのアクセス権を作成する
     * @tparam classtype クラスの型
     * @param type アクセス権を作成したいクラス
     */
    template <class classtype>
    void MakePermissionToContainer(classtype *type)
    {
        //そのクラスのアクセス権は存在しているか
        if (!CheckPermissionToContainer(type))
        {
            int classcount = static_cast<int>(m_listClassType.size());
            m_listClassType[typeid(classtype)] = classcount;
        }
    }

    /**
     * @brief クラスのコンテナへのアクセス権を確認するメソッド
     * @tparam classtype 確認したいクラスの型
     * @param type 確認したいクラスのポインタ
     * @return true 存在する
     * @return false 存在しない
     */
    template <class classtype>
    [[nodiscard]] bool CheckPermissionToContainer(classtype *type)
    {
        return m_listClassType.contains(typeid(classtype));
    }

    /**
     * @brief リストへのアクセス権(リスト自体)を作成するメソッド
     * @n ほかのクラスから名前さえ合っていればすでに作成したものにもアクセス可能
     * @tparam classtype アクセス権を付与したいクラスの型情報
     * @param type アクセスもとのポインタ
     * @param listname 取得したいリストの名前
     * @return true アクセス権の作成に成功
     * @return false アクセス権を持つクラスがすでにあるので失敗
     */
    template <class classtype>
    bool MakePermissionToList(classtype *type, std::string_view listname)
    {
        //typeはコンテナへのアクセス権を保持しているか
        if (CheckPermissionToContainer(type))
        {
            //アクセスしたいリストは存在するか
            if (!CheckPermissionToList(type, listname.data()))
            {
                int id = m_listClassType[typeid(classtype)];

                m_listAccessData[listname.data()] = id;
                //リストの作成
                m_listResources[listname.data()] = std::make_shared<CListResource>();
                return true;
            }
        }
        return false;
    }

    /**
     * @brief そのリストへのアクセス権を持つかどうかの確認
     * @tparam classtype そのクラスの型情報
     * @param type クラスのポインタ
     * @param listname 確認したいリストの名前
     * @return true アクセス権はある
     * @return false リストへのアクセス権はない or リストが存在しない or アクセス権を持つクラスが違う
     */
    template <class classtype>
    [[nodiscard]] bool CheckPermissionToList(classtype const *type, std::string_view listname)
    {
        //そのクラスにコンテナへのアクセス権はあるか
        if (CheckPermissionToContainer(type))
        {
            //型からIDを取得
            int id = m_listClassType[typeid(classtype)];

            //そのリストは存在するか
            if (CheckList(listname.data()))
            {
                //リストにアクセスできる型のぶんまわし
                for (auto &itr : m_listAccessData)
                {
                    //登録されているIDとアクセス権から取得したIDが一致した
                    if (itr.second == id)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    /**
     * @brief 引数のリストは存在するかどうかの確認
     * @n アクセス権は確認しないので注意!
     * @param listname 確認したいリスト名前
     * @return true 存在する
     * @return false 存在しない
     */
    [[nodiscard]] bool CheckList(std::string_view listname)
    {
        return m_listAccessData.contains(listname.data());
    }

    /**
     * @brief リソースデータを格納しているリストを取得する
     * @tparam classtype 取得するクラス（
     * @param type アクセス権を持つクラスのポインタ
     * @param listname 取得したいリストの名前
     * @return CListResource const* リストへのポインタ
     * @n nullptrならアクセス権はない
     */
    template <class classtype>
    [[nodiscard]] CListResource *GetListResource(classtype const *type, std::string_view listname)
    {
        //そのリストへのアクセス権はあるか
        if (CheckPermissionToList(type, listname.data()))
        {
            return m_listResources[listname.data()].get();
        }
        return nullptr;
    }
};
