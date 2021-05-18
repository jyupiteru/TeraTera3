/**
 * @file CCollision3DSystem.h
 * @author jupiter ()
 * @brief CCollision3DSystemクラスの宣言の記載されたヘッダ
 */

#include <unordered_map>
#include <functional>
#include <DirectXMath.h>

#include "CollisionConcept.h"
#include "../../Core/Helper/ConceptIsComponent.h"
#include "../../../Systems/CVector.h"

#pragma once

class GameObject;
class ComponentBase;
class ComTransform;
class CCollisionBase;

enum class E_COLLISION3D_EVENT
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
     * @brief オブジェクト同士が当たった瞬間
     */
    COLLISION_ENTER,

    /**
     * @brief オブジェクトが触れているとき
     */
    COLLISION_STAY,

    /**
     * @brief 最大値
     */
    MAX
};

/**
 * @brief 3Dの当たり判定を管理するクラス
 */
class CCollision3DSystem
{
    /**
     * @brief 当たり判定を管理するリスト
     * @n オブジェクトID、当たり判定の種類、各コンポーネントID、当たり判定の関数ポインタの順番
     */
    static std::unordered_map<int, std::unordered_map<E_COLLISION3D_EVENT, std::unordered_map<int, std::function<void(GameObject *)>>>> m_ListCollisionFunction;

    /**
     * @brief 3Dの当たり判定をとるオブジェクトのリスト
     * @details オブジェクトID-tuple(オブジェクト(通知用)、サイズ取得用、当たり判定調整用)
     */
    static std::unordered_map<int, std::pair<GameObject *, CCollisionBase *>> m_list3DCollisionObjects;

    static unsigned int m_collisionCounter;

    static float m_collisionTime;

    static unsigned int m_reachMax;

public:
    CCollision3DSystem(){};
    ~CCollision3DSystem()
    {
        Uninit();
    }

    void Uninit();

    void Update();

    void ImGuiDraw(unsigned int windowid);

    void ImGuiDraw_Details(unsigned int windowid);

    void ImGuiDraw_CollisionObjects(unsigned int windowid);

    void ImGuiDraw_CollisionObjectDetails(unsigned int windowid, int objid);

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
        AddOnTriggerEnter3D(objid, component);
        AddOnTriggerStay3D(objid, component);
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

            //そのObjectIdの場所を探す
            for (auto itr = m_ListCollisionFunction[objid].begin(); itr != m_ListCollisionFunction[objid].end();)
            {
                //コンポーネントは登録していたか
                if (itr->second.contains(componentid))
                {
                    itr->second.clear();

                    itr = m_ListCollisionFunction[objid].erase(itr);
                }
                else
                {
                    itr++;
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
     * @param component 登録したいcollderコンポーネント
     */
    template <IsColliderComponent type>
    void SetCollisionObject(GameObject *obj, type *component)
    {
        auto id = GetObjectID(obj);
        m_list3DCollisionObjects[id] = std::make_pair(obj, component);
    }

    /**
     * @brief 登録したオブジェクトを破棄する処理
     * @param objid 破棄したいオブジェクトの情報
     */
    void EraseCollisionObject(int objid);

private:
    /**
     * @brief Get the Object I D
     * @details 循環参照対策
     * @param *obj idを取得したいオブジェクトのアドレス
     * @return int オブジェクトのid
     */
    [[nodiscard]] int GetObjectID(GameObject *obj);

    /**
     * @brief 当たった時に呼び出すメソッド
     * @param target 関数を呼びしたいほうのオブジェクト
     * @param hitobject 渡したい（当たった）オブジェクト
     * @param type 当たり判定の種類
     */
    void RunCollisionEvent(GameObject *target, GameObject *hitobject, E_COLLISION3D_EVENT type);

    /**
     * @brief OnTriggerEnter3Dがコンポーネント内にある時入る処理
     * @tparam type コンポーネントのタイプ
     * @param objid オブジェクトのID
     * @param component OnTriggerEnter3Dを発見したコンポーネント
     */
    template <Has_OnTriggerEnter3D type>
    void AddOnTriggerEnter3D(int objid, type *component)
    {
        auto componentid = component->m_componentID.GetValue();
        //コンテナに直接構築
        m_ListCollisionFunction[objid][E_COLLISION3D_EVENT::TRIGGER_ENTER].emplace(componentid, std::bind(&type::OnTriggerEnter3D, component, std::placeholders::_1));
    }

    /**
     * @brief OnTriggerEnter3Dがコンポーネント内にある時入る処理
     * @tparam type コンポーネントのタイプ
     * @param objid オブジェクトのID
     * @param component OnTriggerEnter3Dを発見したコンポーネント
     */
    template <Has_OnTriggerStay3D type>
    void AddOnTriggerStay3D(int objid, type *component)
    {
        auto componentid = component->m_componentID.GetValue();
        //コンテナに直接構築
        m_ListCollisionFunction[objid][E_COLLISION3D_EVENT::TRIGGER_STAY].emplace(componentid, std::bind(&type::OnTriggerStay3D, component, std::placeholders::_1));
    }

    /**
     * @brief OnCollisionEnter3Dがコンポーネント内にある時入る処理
     * @tparam type コンポーネントのタイプ
     * @param objid オブジェクトのID
     * @param component OnCollisionEnter3Dを発見したコンポーネント
     */
    template <Has_OnCollisionEnter3D type>
    void AddOnCollisionEnter3D(int objid, type *component)
    {
        auto componentid = component->m_componentID.GetValue();
        //コンテナに直接構築
        m_ListCollisionFunction[objid][E_COLLISION3D_EVENT::COLLISION_ENTER].emplace(componentid, std::bind(&type::OnCollisionEnter3D, component, std::placeholders::_1));
    }

    /**
     * @brief OnCollisionStay3Dがコンポーネント内にある時入る処理
     * @tparam type コンポーネントのタイプ
     * @param objid オブジェクトのID
     * @param component OnCollisionStay3Dを発見したコンポーネント
     */
    template <Has_OnCollisionStay3D type>
    void AddOnCollisionStay3D(int objid, type *component)
    {
        auto componentid = component->m_componentID.GetValue();
        //コンテナに直接構築
        m_ListCollisionFunction[objid][E_COLLISION3D_EVENT::COLLISION_STAY].emplace(componentid, std::bind(&type::OnCollisionStay3D, component, std::placeholders::_1));
    }

    //失敗用処理
    /**
     * @brief OnTriggerEnter3Dがコンポーネントのなかにないときに通る処理
     * @param ... 何が入ってもいい
     */
    void AddOnTriggerEnter3D(...){};

    /**
     * @brief AddOnCollisionStay3Dがコンポーネントのなかにないときに通る処理
     * @param ... 何が入ってもいい
     */
    void AddOnCollisionStay3D(...){};

    /**
     * @brief OnCollisionEnter3Dがコンポーネントのなかにないときに通る処理
     * @param ... 何が入ってもいい
     */
    void AddOnCollisionEnter3D(...){};

    /**
     * @brief AddOnTriggerStay3Dがコンポーネントのなかにないときに通る処理
     * @param ... 何が入ってもいい
     */
    void AddOnTriggerStay3D(...){};

    /**
     * @brief SetCollisionObjectがコンポーネントのなかにないときに通る処理
     * @param ...
     */
    void SetCollisionObject(...){};

    /**
     * @brief 当たり判定をとる処理
     * @param obj1collider 1個目のオブジェクトの当たり判定管理
     * @param obj2collider 2個目のオブジェクトの当たり判定管理
     * @return true 当たっている
     * @return false 当たっていない
     */
    [[nodiscard]] bool CheckCollisionDetection(const CCollisionBase &obj1collider, const CCollisionBase &obj2collider);

    /**
     * @brief コンポーネントの当たり判定の関数を呼び出す処理
     * @param obj1 当たり判定をとりたいオブジェクト
     * @param obj2 当たった時に渡したいオブジェクト
     * @param type どの当たり判定の種類か
     */
    void RunCollisionDetection(GameObject *obj1, GameObject *obj2, E_COLLISION3D_EVENT type);

    /**
     * @brief 箱と箱の当たり判定をとる処理 OBB
     * @param obj1collider 1個目のオブジェクトの当たり判定管理するクラス
     * @param obj2collider 2個目のオブジェクトの当たり判定管理するクラス
     * @return true 当たっている
     * @return false 当たっていない
     */
    [[nodiscard]] bool Collision_BoxAndBox(const CCollisionBase &obj1collider, const CCollisionBase &obj2collider);

    /**
     * @brief 球と球の当たり判定をとる処理 OBB
     * @param obj1collider 1個目のオブジェクトの当たり判定管理するクラス
     * @param obj2collider 2個目のオブジェクトの当たり判定管理するクラス
     * @return true 当たっている
     * @return false 当たっていない
     * @todo まだ実装書いてないので要追加
     */
    [[nodiscard]] bool Collision_SphereAndSphere(CCollisionBase *obj1collider, CCollisionBase *obj2collider);

    /**
     * @brief ???
     * @param collider1 1個目のオブジェクトの当たり判定管理するクラス
     * @param collider2 2個目のオブジェクトの当たり判定管理するクラス
     * @param pvecSeparate 分離軸
     * @param pvecDistance オブジェクトどうしの距離(ベクトル)
     * @return true 当たっている
     * @return false 当たっていない
     */
    [[nodiscard]] bool CompareLength(const CCollisionBase &collider1, const CCollisionBase &collider2,
                                     const DirectX::XMFLOAT3 &pvecSeparate, const DirectX::XMFLOAT3 &pvecDistance);
};
