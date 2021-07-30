/**
 * @file ComAssimpCollider.cpp
 * @author jupiter ()
 * @brief ComAssimpColliderの実装が書かれたcpp
 */
#include "ComAssimpCollider.h"
#include "../../../Core/GameObject.h"
#include "../../Behavior/Com3DModelAssimp/Com3DModelAssimp.h"

void ComAssimpCollider::Init()
{
}

void ComAssimpCollider::Ready()
{
    if (m_gameObject->GetComponent<Com3DModelAssimp>())
    {
        MakeAssimpCollider();
    }
}

void ComAssimpCollider::Update()
{
}
void ComAssimpCollider::Draw()
{
}

void ComAssimpCollider::MakeAssimpCollider(void)
{
    auto mesh = m_gameObject->GetComponent<Com3DModelAssimp>()->GetModelData().modeldata.GetMeshes();

    for (auto& itr : mesh)
    {
    }

    //todo 当たり判定を構築する処理を追加!
}