/**
 * @file EventSystem.h
 * @author jupiter ()
 * @brief CheckEventクラスが記述されたヘッダ
 */

#include "../Core/Helper/ConceptIsComponent.h"

#include "Collision/CCollision2DSystem.h"
#include "Collision/CCollision3DSystem.h"

#include "Collision/CollisionConcept.h"

#pragma once

#include <memory>

/**
 * @brief イベントに各処理登録時に使用
 */
enum class E_TYPE_EVENT
{
    COLLISION2D = 0,
    COLLISION3D = 1,
};

class GameObject;
class ComponentBase;

/**
 * @brief 各イベント（当たり判定等）を管理するクラス（ほぼシングルトン）
 */
class EventSystem final
{
    static std::unique_ptr<CCollision2DSystem> m_collision2D;
    static std::unique_ptr<CCollision3DSystem> m_collision3D;

public:
    EventSystem()
    {
        m_collision2D = std::make_unique<CCollision2DSystem>();
        m_collision3D = std::make_unique<CCollision3DSystem>();
    };

    ~EventSystem()
    {
        m_collision2D.reset();
        m_collision3D.reset();
    };

    void Update()
    {
        m_collision2D->Update();
        m_collision3D->Update();
    }

    void ImGuiDraw(unsigned int windowid);

    /**
     * @brief 特定のイベント関数がコンポーネントに含まれているか確認するメソッド
     * @tparam classtype 判断したいコンポーネント
     * @param obj 親オブジェクトのアドレス
     * @param component 判定したいコンポーネントのアドレス（継承後のもの）
     */
    template <IsComponent classtype>
    static void SetComponentToEvent(GameObject *obj, classtype *component)
    {
        //当たり判定システムにコンポーネント情報を登録
        //m_collision2D->SetCollisionFunction(obj, component);

        m_collision3D->SetCollisionFunction(obj, component);
        //以下随時何かあれば追加
    }

    /**
     * @brief イベントがついているコンポーネントをリストから削除する処理
     * @tparam classtype 削除したいコンポーネントの種類
     * @param obj 破棄したいコンポーネントを持つオブジェクト
     * @param component 破棄したいコンポーネント
     */
    template <IsComponent classtype>
    static void EraseComponentFromEvent(GameObject *obj, classtype *component)
    {
        //当たり判定システムにコンポーネント情報から逆残して削除
        //m_collision2D->DestroyCollisionFunction(obj, component);

        m_collision3D->DestroyCollisionFunction(obj, component);
        //以下随時何かあれば追加
    }

    /**
     * @brief 各イベントにオブジェクトを登録する処理
     * @param object 登録したいオブジェクト
     * @param type 登録したいイベントの種類
     * @param component 渡したいこのコンポーネントの引数等
     */
    template <IsComponent classtype>
    static void SetObjectToEvent(GameObject *object, E_TYPE_EVENT type, classtype *component)
    {
        switch (type)
        {
        case E_TYPE_EVENT::COLLISION2D:
            //m_collision2D->SetCollisionObject(object, component);
            break;

        case E_TYPE_EVENT::COLLISION3D:
            m_collision3D->SetCollisionObject(object, component);
            break;

        default:
            break;
        }
    }

    /**
     * @brief 各イベントに登録したオブジェクトを削除する処理
     * @param object 削除したいオブジェクトのアドレス
     * @param type どのイベントから削除するか
     */
    static void EraseObjectFromEvent(int objectid, E_TYPE_EVENT type);

    /**
     * @brief イベントを一括で削除する処理
     * @param object 削除したいオブジェクトのアドレス
     */
    static void EraseObjectFromEvent(int objectid);
};
