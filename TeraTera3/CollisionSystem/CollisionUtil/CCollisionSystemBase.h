/**
 * @file CCollisionSystemBase.h
 * @author jupiter ()
 * @brief
 */

#include <unordered_map>
#include <functional>
#include "../CollisionConcept.h"
#include "../../ComSystem/Core/helper/ConceptIsComponent.h"
#include "../../System/CVector.h"

#pragma once

enum class E_COLLISION_EVENT
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

class GameObject;

class CCollisionSystemBase
{
protected:
    /**
     * @brief 当たり判定を管理するリスト
     * @n オブジェクトID、当たり判定の種類、各コンポーネントID、当たり判定の関数ポインタの順番
     */
    std::unordered_map<int, std::unordered_map<E_COLLISION_EVENT, std::unordered_map<int, std::function<void(GameObject *)>>>> m_ListCollisionFunction;

    /**
     * @brief 当たり判定をとるオブジェクトのリスト
     * @details オブジェクトID-tuple(オブジェクト(通知用)、サイズ取得用)
     */
    std::unordered_map<int, std::pair<GameObject *, CCollisionBase *>> m_listCollisionObjects;

    unsigned int m_collisionCounter = 0;

    float m_collisionTime = 0.0f;

    unsigned int m_reachMax = 0;

public:
    CCollisionSystemBase(){};
    virtual ~CCollisionSystemBase() { Uninit(); };

    void Uninit();

    void Update() { CollissionFunction(); };

    /**
     * @brief Update内で行われる衝突判定の処理
     * @n 各継承クラスで再定義すること
     */
    virtual void CollissionFunction() = 0;

    //ImGui系
    void ImGuiDraw(unsigned int windowid);

    void ImGuiDraw_Details(unsigned int windowid);

    void ImGuiDraw_CollisionObjects(unsigned int windowid);

    void ImGuiDraw_CollisionObjectDetails(unsigned int windowid, int objid);
};