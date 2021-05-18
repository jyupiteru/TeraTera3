/**
 * @file PackageBase.h
 * @author jupiter
 * @brief パッケージクラスのベースが記述されているヘッダ
 */

#include "ComponentBase.h"
#include "GameObject.h"
#include"Helper/ConceptIsComponent.h"

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#pragma once

/**
 * @brief コンポーネントをまとめたパッケージクラスの親クラス
 */
class PackageBase
{
    /**
     * @brief このパッケージのコンポーネントリスト
     */
    std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> m_pListComponent;

public:
    /**
     * @brief  CBasePackagesの生成時処理
     */
    PackageBase(){};

    /**
     * @brief CBasePackagesの破棄時処理
     */
    virtual ~PackageBase(){};

    /**
     * @brief   コンポーネントを全部セットする処理 継承先でセットする
     * @details overrideによりオーバーライド必須
     */
    virtual void Init(){
        //AddComponent<>();
        //AddPackage<>();
    };

    /**
     * @brief   コンポーネントをObjectにセットするのに使う処理
     * @return std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>>* セットするリスト
     */
    std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> *GetListComponent()
    {
        return &m_pListComponent;
    }

    /**
     * @brief  コンポーネントをセットする処理
     * @tparam com 入れたいコンポーネントクラスの型を入れる
     */
    template <IsComponent com>
    void AddComponent()
    {
        //すでに追加されてないかの確認処理
        if (!m_pListComponent.contains(typeid(com)))
        {
            std::shared_ptr<com> component = std::make_shared<com>();
            m_pListComponent[typeid(com)] = component; //ここでエラーが起こったのならcomponentを継承していないクラスが入っている
            component->SetComponentID(component.get());
        }
    }

    /**
     * @brief       コンポーネントをまとめたpackageを追加する処理
     * @tparam T    セットしたいパッケージを選択
     */
    template <class T>
    void AddPackage()
    {
        std::shared_ptr<T> package = std::make_shared<T>();
        package->Init();
        std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> *packagelist = package->GetListComponent();

        //c++17から追加機能
        //エラーが出たらプロジェクト=>c/c++->言語標準からc++17にすること
        //自信にないキーを追加する
        m_pListComponent.merge(*packagelist);
    }
};
