/**
 * @file ComAssimpCollider.cpp
 * @author jupiter ()
 * @brief ComAssimpColliderの実装が書かれたcpp
 */
#include "ComAssimpCollider.h"
#include "../../../Core/GameObject.h"
#include "../../Behavior/Com3DModelAssimp/Com3DModelAssimp.h"
#include "../../Behavior/ComTransform/ComTransform.h"

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

    //auto rate = m_gameObject->m_transform->m_sizeRateSize.GetValue();

    //{ //当たり判定の大きさをモデルと一緒にする
    //    auto [firstSize_x, firstSize_y, firstSize_z] = m_gameObject->m_transform->m_offsetSize.GetValue();
    //    firstSize_x *= rate;
    //    firstSize_y *= rate;
    //    firstSize_z *= rate;

    //    m_finalSize.x = size_x * firstSize_x + offsize_x;
    //    m_finalSize.y = size_y * firstSize_y + offsize_y;
    //    m_finalSize.z = size_z * firstSize_z + offsize_z;
    //}

    //auto mesh = m_gameObject->GetComponent<Com3DModelAssimp>()->GetModelData().modeldata.GetMeshes();

    //for (auto &itr : mesh)
    //{
    //    itr.m_listBoneVertices;
    //}

    //todo 当たり判定を構築する処理を追加!
}