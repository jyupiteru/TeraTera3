/**
 * @file CCollisionSystem2D.h
 * @author jupiter ()
 * @brief CCollisionSystem2Dの記述されたヘッダ
 */

#include <unordered_map>
#include <functional>

#include "CollisionConcept.h"
#include "../ComSystem/Core/helper/ConceptIsComponent.h"
#include "../System/CVector.h"

#pragma once

class GameObject;
class ComponentBase;
class ComTransform;
class CCollisionBase;

enum class E_COLLISION2D_EVENT
{
    /**
     * @brief オブジェクト同士が当たった瞬間
     */
    TRIGGER_ENTER = 0,

    /**
     * @brief オブジェクト同士が当たっている間
     */
    TRIGGER_STAY,

    /**
     * @brief 最大値
     */
    MAX
};

/**
 * @brief 2Dの当たり判定を管理するリスト
 * @n 未完成なので注意
 */
class CCollision2DSystem final
{

    /**
     * @brief 当たり判定を管理するリスト
     * @n オブジェクトID、当たり判定の種類、各コンポーネントID、当たり判定の関数ポインタの順番
     */
    static std::unordered_map<int, std::unordered_map<E_COLLISION2D_EVENT, std::unordered_map<int, std::function<void(GameObject *)>>>> m_ListCollisionFunction;

    /**
     * @brief 2Dの当たり判定をとるオブジェクトのリスト
     * @details オブジェクトID-tuple(オブジェクト(通知用)、サイズ取得用、当たり判定調整用)
     */
    static std::unordered_map<int, std::pair<GameObject *, CCollisionBase *>> m_list2DCollisionObjects;

public:
    CCollision2DSystem(){};
    ~CCollision2DSystem()
    {
        Uninit();
    }

    void Uninit();

    void Update();

    /**
     * @brief 当たり判定があるか判断するためのメソッド
     * @tparam componenttype コンポーネントを継承している
     * @param obj オブジェクトのアドレス
     * @param component 判定したいコンポーネントのアドレス
     */
    template <IsComponent componenttype>
    void SetCollisionFunction(GameObject *obj, componenttype *component)
    {
        int objid = GetObjectID(obj);

        //以下随時当たり判定判断する関数追加
        AddOnTriggerEnter2D(objid, component);
        AddOnTriggerStay2D(objid, component);
        //当たり判定をこの中でとるためにゲームオブジェクトを登録する処理？がいる
    }

    /**
     * @brief 特定のコンポーネントの当たり判定を削除する処理
     * @tparam componenttype コンポーネントを継承している
     * @param obj オブジェクトのアドレス
     * @param component 消したいコンポーネントのアドレス
     */
    template <IsComponent componenttype>
    void DestroyCollisionFunction(GameObject *obj, componenttype *component)
    {
        //このオブジェクトは当たり判定を持つとして登録されているか
        if (auto objid = GetObjectID(obj); m_ListCollisionFunction.contains(objid))
        {
            auto componentid = component->m_componentID.GetValue();

            for (auto itr = m_ListCollisionFunction[objid].begin(); itr != m_ListCollisionFunction[objid].end();)
            {
                if (itr->second.contains(componentid))
                {
                    itr->second.clear();

                    //この当たり判定の種類がないなら消す
                    if (m_ListCollisionFunction[objid][itr->first].size() == 0)
                    {
                        itr = m_ListCollisionFunction[objid].erase(itr);
                    }
                }
            }
            //このオブジェクトの当たり判定が残っていないなら消す
            if (m_ListCollisionFunction[objid].size() == 0)
            {
                m_ListCollisionFunction.erase(objid);
            }
        }
    }

    /**
     * @brief 当たり判定を行うオブジェクトを登録する処理
     * @details 当たり判定の関数を登録する処理は別
     * @param obj 登録したいオブジェクトのアドレス
     * @param component セットしたいcolliderのコンポーネント
     */
    template <IsColliderComponent type>
    void SetCollisionObject(GameObject *obj, type *component)
    {
        auto id = GetObjectID(obj);
        m_list2DCollisionObjects[id] = std::make_pair(obj,
                                                      //obj->GetComponent<ComTransform>(),
                                                      component);
    }

    /**
     * @brief 登録したオブジェクトを破棄する処理
     * @param obj 破棄したいオブジェクトの情報
     */
    void EraseCollisionObject(int objid);

private:
    /**
     * @brief Get the Object I D
     * @details 循環参照対策
     * @param *obj idを取得したいオブジェクトのアドレス
     * @return int オブジェクトのid
     */
    int GetObjectID(GameObject *obj);

    /**
     * @brief 当たった時に呼び出すメソッド
     * @param target 関数を呼びしたいほうのオブジェクト
     * @param hitobject 渡したい（当たった）オブジェクト
     * @param type 当たり判定の種類
     * @todo まだどこでも呼び出していないので注意
     */
    void RunCollisionEvent(GameObject *target, GameObject *hitobject, E_COLLISION2D_EVENT type);

    /**
     * @brief OnTriggerEnter2Dがコンポーネント内にある時入る処理
     * @tparam type コンポーネントのタイプ
     * @param objid オブジェクトのID
     * @param component OnTriggerEnter2Dを発見したコンポーネント
     */
    template <Has_OnTriggerEnter2D type>
    void AddOnTriggerEnter2D(int objid, type *component)
    {
        using enum E_COLLISION2D_EVENT;
        auto componentid = component->m_componentID.GetValue();
        //コンテナに直接構築
        m_ListCollisionFunction[objid][TRIGGER_ENTER].emplace(componentid, std::bind(&type::OnTriggerEnter2D, component, std::placeholders::_1));
    }

    /**
     * @brief OnTriggerEnter2Dがコンポーネント内にある時入る処理
     * @tparam type コンポーネントのタイプ
     * @param objid オブジェクトのID
     * @param component OnTriggerEnter2Dを発見したコンポーネント
     */
    template <Has_OnTriggerStay2D type>
    void AddOnTriggerStay2D(int objid, type *component)
    {
        using enum E_COLLISION2D_EVENT;
        auto componentid = component->m_componentID.GetValue();
        //コンテナに直接構築
        m_ListCollisionFunction[objid][TRIGGER_STAY].emplace(componentid, std::bind(&type::OnTriggerStay2D, component, std::placeholders::_1));
    }

    //失敗用処理
    /**
     * @brief OnTriggerEnter2Dがコンポーネントのなかにないときに通る処理
     * @param ... 何が入ってもいい
     */
    void AddOnTriggerEnter2D(...){};

    /**
     * @brief AddOnTriggerEnter2Dがコンポーネントのなかにないときに通る処理
     * @param ... 何が入ってもいい
     */
    void AddOnTriggerStay2D(...){};

    /**
     * @brief SetCollisionObjectが通らなかった場合に通る処理
     * @param ... 何が入ってもいい
     */
    void SetCollisionObject(...){};

    /**
     * @brief 当たり判定をとる処理
     * @param obj1 1個目のオブジェクトのポインタ
     * @param obj2 2個目のオブジェクトのポインタ
     * @param obj1collider 1個目のオブジェクトの当たり判定管理
     * @param obj2collider 2個目のオブジェクトの当たり判定管理
     * @return true
     * @return false
     */
    bool CheckCollisionDetection(GameObject *obj1, GameObject *obj2,
                                 CCollisionBase *obj1collider, CCollisionBase *obj2collider);

    /**
     * @brief コンポーネントの当たり判定の関数を呼び出す処理
     * @param obj1 当たり判定をとりたいオブ弱と
     * @param obj2 当たった時に渡したいオブジェクト
     * @param type どの当たり判定の種類か
     */
    void RunCollisionDetection(GameObject *obj1, GameObject *obj2, E_COLLISION2D_EVENT type);
};
