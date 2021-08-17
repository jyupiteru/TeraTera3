/**
 * @file ComAssimpCollider.h
 * @author jupiter ()
 * @brief ComAssimpColliderの宣言が書かれたヘッダ
 */

#include "../../../Core/ComponentBase.h"
#include "unordered_map"

#pragma once


/**
 * @brief Assimpモデルに細かく当たり判定をつける際に使用するコンポーネント
 */
class ComAssimpCollider : public ComponentBase
{
    std::unordered_map<std::string, GameObject *> m_listCollliderObject;

public:
    virtual void Init();
    virtual void Uninit();
    virtual void Ready();
    virtual void Update();
    virtual void Draw();

private:
    void MakeAssimpCollider(void);

};
