/**
 * @file RigidBody3D.h
 * @author jupiter ()
 * @brief ComRigidBody3Dを記述しているヘッダ
 */
#include "../../../Core/Cores.h"

#pragma once

/**
 * @brief 物理演算の挙動を追加するコンポーネント
 * @todo ここ随時追加
 */
class ComRigidBody3D : public ComponentBase
{

public:
    
    /**
     * @brief このオブジェクトの重さ
     */
    float m_weight = 1.0f;
    
    /**
     * @brief 重力を適用するか
     */
    bool m_flagUseGravity = false;

    ComRigidBody3D(){};

    ~ComRigidBody3D(){};

    void Update() override;
};
