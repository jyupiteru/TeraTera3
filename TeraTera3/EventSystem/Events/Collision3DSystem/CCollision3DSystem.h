/**
 * @file CCollision3DSystem.h
 * @author jupiter ()
 * @brief CCollision3DSystemクラスの宣言の記載されたヘッダ
 */

#include <unordered_map>
#include <functional>
#include <DirectXMath.h>

#include "../CollisionUtil/CollisionConcept.h"
#include "../../../ComSystem/Core/helper/ConceptIsComponent.h"
#include "../../../System/CVector.h"

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
    std::unordered_map<int, std::unordered_map<E_COLLISION3D_EVENT, std::unordered_map<int, std::function<void(GameObject *)>>>> m_ListCollisionFunction;

    /**
     * @brief 衝突時に渡すオブジェクトのペア
     * @n ID,オブジェクト
     */
    std::unordered_map<int, GameObject *> m_listObject;

    /**
     * @brief 3Dの当たり判定をとるオブジェクトのリスト
     * @n オブジェクトID-＜部位名、その部位の衝突判定の情報＞
     */
    std::unordered_map<int, std::unordered_map<std::string, CCollisionBase *>> m_list3DCollisionObjects;

    /**
     * @brief ImGuiでの総衝突回数描画用変数
     */
    unsigned int m_collisionCounter = 0;

    /**
     * 衝突にかかった時間を表示する用の変数
    */
    float m_collisionTime = 0.0f;

    /**
     * @brief 衝突回数の上限に達した回数
     */
    unsigned int m_reachMax = 0;

    static CCollision3DSystem *m_instance;

public:
    CCollision3DSystem(){};
    ~CCollision3DSystem();
    void Uninit();
    void Update();
    static void Create();
    static void Delete(bool _flag = false);
    static [[nodiscard]] CCollision3DSystem &GetInstance();

    void ImGuiDraw(unsigned int windowid);

    void ImGuiDrawDetails(unsigned int windowid);

    void ImGuiDrawCollisionObjects(unsigned int windowid);

    void ImGuiDrawCollisionObjectDetails(unsigned int windowid, int objid);

    /**
     * @brief 当たり判定時の処理があるか判断するためのメソッド
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
        //当たり判定をこの中でとるためにゲームオブジェクトを登録する処理がいる
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
     * @param _obj 当たり判定の関数を登録する処理は別
     * @param _collidername 衝突判定範囲の名前？ これによって区分けする(異なれば複数登録できる)
     * @param _component 登録したいcollderコンポーネント
     */
    template <IsColliderComponent type>
    void SetCollisionObject(GameObject *_obj, std::string _collidername, type *_component)
    {
        auto id = GetObjectID(_obj);
        m_list3DCollisionObjects[id][_collidername] = _component;

        if (!m_listObject.contains(id))
        { //未登録なので
            m_listObject[id] = _obj;
        }
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

    /**
     * @brief 当たり判定をとる処理
     * @param obj1collider 1個目のオブジェクトの当たり判定管理
     * @param obj2collider 2個目のオブジェクトの当たり判定管理
     * @return true 当たっている
     * @return false 当たっていない
     */
    [[nodiscard]] bool CheckCollisionDetection(const CCollisionBase &obj1collider, const CCollisionBase &obj2collider);

    /**
     * @brief コンポーネントの衝突判定時の関数を呼び出す処理
     * @param obj1id 衝突されたオブジェクト
     * @param obj2id 衝突したオブジェクト
     * @param type どの種類の判定か？
     */
    void RunCollisionDetection(int obj1id, int obj2id, E_COLLISION3D_EVENT type);

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

    //失敗用処理
    void AddOnTriggerEnter3D(...){};
    void AddOnCollisionStay3D(...){};
    void AddOnCollisionEnter3D(...){};
    void AddOnTriggerStay3D(...){};
    void SetCollisionObject(...){};
};
