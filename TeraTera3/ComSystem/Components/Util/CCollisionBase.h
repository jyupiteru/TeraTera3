/**
 * @file CCollisionBase.h
 * @author jupiter ()
 * @brief Collisionコンポーネント系の必要なもの集めたヘッダ
 */

#include "../../../System/CVector.h"
#include <DirectXMath.h>

#pragma once

/**
 * @brief 当たり判定の分類に使用
 */
enum class E_TYPE_COLLISIONCLASS
{
    /**
     * @brief 四角形
     */
    BOX = 0,

    /**
     * @brief 円
     */
    SPHERE
};

class CCollision2DSystem;
class CCollision3DSystem;

/**
 * @brief 当たり判定の共通する部分を保持するのに使用
 */
class CCollisionBase
{
protected:
    /**
     * @brief 今当たっているかの情報を保持する変数
     */
    bool m_state = false;

    /**
     * @brief この当たり判定の種類
     * @details 継承後変更すること
     */
    E_TYPE_COLLISIONCLASS m_typeCollision = E_TYPE_COLLISIONCLASS::BOX;

    /**
     * @brief 当たり判定で使用する行列
     * @details 実際のモデル等の大きさと異なる場合があるので使用
     */
    DirectX::XMFLOAT4X4 m_colliderMatrix;

    /**
     * @brief ローカルのx軸はどの方向に向いているか
     */
    DirectX::XMFLOAT3 m_vecAxisX = {1.0f, 0.0f, 0.0f};

    /**
     * @brief ローカルのy軸はどの方向に向いているか
     */
    DirectX::XMFLOAT3 m_vecAxisY = {0.0f, 1.0f, 0.0f};

    /**
     * @brief ローカルのz軸はどの方向に向いているか
     */
    DirectX::XMFLOAT3 m_vecAxisZ = {0.0f, 0.0f, 1.0f};

    /**
     * @brief 最終の大きさ
     * @todo もしかしたらCMatrixみたいに行列から取り出したら不要になるかも
     */
    DirectX::XMFLOAT3 m_finalSize = {1.0f, 1.0f, 1.0f};

    /**
     * @brief 最も大きいsize
     */
    float m_biggetSize = 0.0f;

public:
    friend CCollision2DSystem;
    friend CCollision3DSystem;

    /**
     * @brief 描画するかどうか
     * @details 描画するならUpdate前にフラグを立てること
     */
    bool m_draw = false;

    /**
     * @brief 初期の当たり判定のサイズをモデルと一緒にするかどうか
     */
    bool m_isFirstJustSize = false;

    /**
     * @brief 座標の補正値
     */
    CVector3<float> m_offsetPosition;

    /**
     * @brief 角度の補正値
     */
    CVector3<float> m_offsetAngle;

    /**
     * @brief 大きさの補正値
     */
    CVector3<float> m_offsetSize;

    /**
     * @brief 描画する際の色
     */
    CVector4<float> m_color;

    /**
     * @brief この当たり判定をフラグにするかどうか falseでRigidbodyを持つとき物理挙動(貫通する）
     * @n なのでOnTrigger系の当たり判定は両方falseにすること
     * @details
     */
    CVector<bool> m_isTrigger;

    virtual void ImGui_Draw(unsigned int windowid);

    //例
    // void OnTriggerStay3D(GameObject *);

    // void OnCollisionStay3D(GameObject *);
};
