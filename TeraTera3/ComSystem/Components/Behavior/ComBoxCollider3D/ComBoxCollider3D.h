/**
 * @file ComBoxCollider3D.h
 * @author jupiter ()
 * @brief ComCollider3Dが記載されたヘッダ
 */
#include "../../../Core/ComponentBase.h"
#include "../../Shape/ComBox/ComBox.h"
#include "../../Util/CCollisionBase.h"
#include "../../../Core/GameObject.h"

#pragma once

class CCollision3DSystem;

/**
 * @brief UIの当たり判定付与や大きさの調整等をするコンポーネント
 */
class ComBoxCollider3D : public ComponentBase, public CCollisionBase
{
    /**
     * @brief 当たり判定用のオブジェクトを保持する変数
     * @details あまり外からいじらないように
     */
    GameObject *m_colliderObject = nullptr;

    ComBox *m_colliderObjectBox = nullptr;

public:
    friend CCollision3DSystem;

    ComBoxCollider3D(){};
    ~ComBoxCollider3D(){};

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Ready() override;

    virtual void Update() override;

    virtual void Draw() override;

    virtual void ImGuiDraw(unsigned int windowid) override;

protected:
    /**
     * @brief 当たり判定の情報と子オブジェクトの位置等を更新する
     */
    void UpdateColliderDataAndChild(void);
};
