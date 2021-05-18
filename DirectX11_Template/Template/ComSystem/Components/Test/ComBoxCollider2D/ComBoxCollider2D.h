///**
// * @file ComCollider2D.h
// * @author jupiter ()
// * @brief ComCollider2Dが記載されたヘッダ
// */
//#include "../../../Core/Cores.h"
//#pragma once
//
//class CCollision2DSystem;
//
///**
// * @brief UIの当たり判定付与や大きさの調整等をするコンポーネント
// */
//class ComCollider2D : public ComponentBase
//{
//    /**
//     * @brief 今当たっているかの情報を保持する変数
//     */
//    bool m_state = false;
//
//public:
//    friend CCollision2DSystem;
//
//    //todo ここにどんな形の当たり判定をとるかのフラグを追加
//
//    /**
//     * @brief 物理挙動管理フラグ trueでRigidbodyを持つとき物理挙動（動かない）
//     * @details
//     */
//    CVector<bool> m_isTrigger;
//
//    ComCollider2D(){};
//    ~ComCollider2D(){};
//
//    void Uninit() override;
//
//    void Ready() override;
//};
