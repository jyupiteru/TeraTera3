/**
 * @file ComponentBase.h
 * @author jupiter
 * @brief コンポーネントの親クラスの記述されたヘッダ
 * @version 1.0
 * @date 2020-08-07
 */

#include <string_view>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Helper/ConceptIsComponent.h"
#include "GameObject.h"
#include "../../System/CVector.h"

#define NOMINMAX

class PackageBase;

#pragma once

enum class E_TYPE_COMPONENT
{
    DEFAULT = 0,
    NONETYPE,
    OBJECT2D,
    OBJECT3D,
    SYSTEM,
};

/**
 * @brief すべてのコンポーネントクラスのベース
 */
class ComponentBase
{
    /**
     * @brief 一度でも生成したコンポーネントクラスの型の種類を保持するリスト
     */
    static std::unordered_map<std::type_index, int> m_listComponentID;

    /**
     * @brief Updateがオーバーライドされているという証拠の変数 されていないとfalseになり更新処理が通らなくなる
     */
    bool m_flagUpdate = true;

    /**
     * @brief Drawがオーバーライドされているという証拠の変数 されていないとfalseになり描画処理が通らなくなる
     */
    bool m_flagDraw = true;

public:
    friend GameObject;
    friend PackageBase;

    /**
     * @brief このコンポーネント（継承後）のID
     * @n 書き換えないこと
     */
    CVector<int> m_componentID;

    /**
     * @brief このコンポーネントはいまアクティブかどうか
     */
    CVector<bool> m_enable;

    /**
     * @brief コンポーネントの種類を保持する変数
     */
    CVector<E_TYPE_COMPONENT> m_typeComponent;

    /**
     * @brief 親オブジェクトのポインタ
     */
    GameObject *m_gameObject = nullptr;

    /**
     * @brief 更新処理と描画処理のオブジェクトのコンポーネント内での優先順位
     */
    CVector<int> m_updateAndDrawOrder;

public:
    /**
     * @brief  CComponentBaseの生成時処理
     */
    ComponentBase()
    {
        m_enable.SetValue(true);
        m_updateAndDrawOrder.SetValue(0);
    };

    /**
     * @brief CComponentBaseの破棄時処理
     */
    virtual ~ComponentBase(){};

    /**
     * @brief 生成されたときに通る処理
     * @details コンストラクタと同じタイミングで呼ばれる
     */
    virtual void Init(){};

    /**
     * @brief 破棄時処理
     */
    virtual void Uninit(){};

    /**
     * @brief 有効になった後Update前で1回だけ通る処理
     */
    virtual void Ready(){};

    /**
     * @brief アップデート処理
     * @n 継承後にオーバーライドされていなければ通らなくなる
     */
    virtual void Update();

    /**
     * @brief 描画処理
     * @n 継承後にオーバーライドされていなければ通らなくなる
     */
    virtual void Draw();

    /**
     * @brief ImGui描画時に表示、変更させたい内容を入れた処理
     */
    virtual void ImGuiDraw(unsigned int windowid);

    /**
     * @brief オブジェクトを削除するメソッド
     * @param obj 削除したいオブジェクトのポインタ
     * @return true 成功
     * @return false 失敗
     */
    bool Destroy(GameObject *obj)
    {
        return GameObject::DestroyObject(obj);
    }

    /**
     * @brief オブジェクトを削除するメソッド2
     * @param name 削除したいオブジェクトの名前
     * @return true 成功
     * @return false 失敗
     */
    bool Destroy(const char *name)
    {
        return GameObject::DestroyObject(name);
    }

    /**
     * @brief オブジェクトを削除するメソッド3
     * @n 一番軽い処理
     * @param id 削除したいオブジェクトのid
     * @return true 成功
     * @return false 失敗
     */
    bool Destroy(int id)
    {
        return GameObject::DestroyObject(id);
    }

    /**
     * @brief 特定のオブジェクトからコンポーネントを削除するメソッド
     * @param component 削除したいコンポーネント(そのオブジェクトが保持しているもの)
     * @return true 成功
     * @return false 失敗
     */
    template <IsComponent type>
    bool Destroy(type *component)
    {
        return m_gameObject->RemoveComponent(component);
    }

private:
    /**
     * @brief Set the Component I D object
     * @tparam type セットしたイコンポーネントの種類
     * @param component セットしたいコンポーネント（ComBaseならおかしい）
     */
    template <IsComponent type>
    void SetComponentID(type *component)
    {
        m_componentID.SetValue(GetComponentID<type>());
    }

    /**
     * @brief Get the Component I D object
     * @tparam type IDを取得したいコンポーネントクラス
     * @return const int コンポーネントID
     */
    template <IsComponent type>
    [[nodiscard]] int GetComponentID(void)
    {
        if (!m_listComponentID.contains(typeid(type)))
        {
            auto total = m_listComponentID.size();
            m_listComponentID[typeid(type)] = static_cast<int>(total);
        }
        return m_listComponentID[typeid(type)];
    }
};