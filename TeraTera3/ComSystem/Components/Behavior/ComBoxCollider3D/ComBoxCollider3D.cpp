/**
 * @file ComBoxCollider3D.cpp
 * @author jupiter ()
 * @brief ComBoxCollider3Dの実装が書かれたcpp
 */
#include "ComBoxCollider3D.h"
#include "../Com3DModelAssimp/Com3DModelAssimp.h"
#include "../../../../System/CMatrix/CMatrix.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../ComTransform/ComTransform.h"
#include "../../../../EventSystem/Events/Collision3DSystem/CCollision3DSystem.h"

void ComBoxCollider3D::Init()
{
    m_typeCollision = E_TYPE_COLLISIONCLASS::BOX;
    m_typeComponent.SetValue(E_TYPE_COMPONENT::SYSTEM);

    m_color.SetValue(256.0f, 256.0f, 256.0f, 0.4f);
}

//================================================================================================
//================================================================================================

void ComBoxCollider3D::Uninit()
{
    CCollision3DSystem::GetInstance().EraseCollisionObject(this->m_gameObject->m_objID);
}

//================================================================================================
//================================================================================================

void ComBoxCollider3D::Ready()
{
    if (m_gameObject->m_typeObject == E_TYPE_OBJECT::MODEL3D)
    {
        //このオブジェクトを当たり判定リストに登録
        CCollision3DSystem::GetInstance().SetCollisionObject(this->m_gameObject,"All", this);

        //todo のちに解凍
        //if (!this->m_gameObject->GetComponent<ComRigidBody3D>())
        //{
        //    //物理挙動はなしなので貫通(トリガー化)
        //    m_isTrigger.SetValue(true);
        //}

        //登録されなければ生成はなし
        if (m_draw)
        {
            m_colliderObject = m_gameObject->AddChildObject(this->m_gameObject->m_objectName + ":3dcollider", E_TYPE_OBJECT::NONE);
            m_colliderObject->m_typeObject = E_TYPE_OBJECT::MODEL3D;
            m_colliderObject->AddComponent<ComShader>();
            m_colliderObjectBox = m_colliderObject->AddComponent<ComBox>();
        }
    }
}

//================================================================================================
//================================================================================================

void ComBoxCollider3D::Update()
{
    //子オブジェクトにした当たり判定をもとに情報を作成
    UpdateColliderDataAndChild();
}

//================================================================================================
//================================================================================================

void ComBoxCollider3D::Draw()
{
    if (m_colliderObject != nullptr)
    {
        if (m_draw)
        {
            //そもそもtransformを通させないことで行列を更新させない(でもtransformの値を取得されたら確実にバグ)
            m_colliderObject->m_transform->m_enable.SetValue(false);

            //Updateの更新順位をこのオブジェクトより後にすることでTransformの書き換えを防ぐ？
            m_colliderObject->m_objectUpdatePriority.SetValue(m_gameObject->m_objectDrawPriority.GetValue() + 1);
            m_colliderObject->m_transform->m_color.SetValue(m_color.GetValue());
        }
        else
        {
            m_colliderObjectBox->m_enable.SetValue(false);
        }
    }
}

//================================================================================================
//================================================================================================

void ComBoxCollider3D::ImGuiDraw(unsigned int windowid)
{
    //ほぼこっちの処理と一緒なので
    CCollisionBase::ImGuiDraw(windowid);

    //描画するか and オブジェクトは存在しているか
    if (m_draw && m_colliderObject != nullptr)
    {
        if (ImGui::TreeNode(m_colliderObject->m_objectName.c_str()))
        {
            m_colliderObject->ImGuiDraw(windowid);
            ImGui::TreePop();
        }
    }
}

//================================================================================================
//================================================================================================

void ComBoxCollider3D::UpdateColliderDataAndChild(void)
{

    //12/30時点でCMatrixを使用する方式に変更しているのでエラーが出たらそれ以前参照!

    CMatrix *mtx = new CMatrix();
    mtx->SetMatrix(m_gameObject->m_transform->GetNoScalingMatrix());

    auto [offsize_x, offsize_y, offsize_z] = m_offsetSize.GetValue();

    auto [size_x, size_y, size_z] = m_gameObject->m_transform->m_size.GetValue();

    auto rate = m_gameObject->m_transform->m_sizeRateSize.GetValue();

    { //大きさの設定
        if (m_isFirstJustSize)
        { //当たり判定の大きさをモデルと一緒にする
            auto [firstSize_x, firstSize_y, firstSize_z] = m_gameObject->m_transform->m_offsetSize.GetValue();
            firstSize_x *= rate;
            firstSize_y *= rate;
            firstSize_z *= rate;

            m_finalSize.x = size_x * firstSize_x + offsize_x;
            m_finalSize.y = size_y * firstSize_y + offsize_y;
            m_finalSize.z = size_z * firstSize_z + offsize_z;
        }
        else
        {
            m_finalSize.x = size_x + offsize_x;
            m_finalSize.y = size_y + offsize_y;
            m_finalSize.z = size_z + offsize_z;
        }

        mtx->SetSize(m_finalSize.x, m_finalSize.y, m_finalSize.z);

        { //最も長い大きさの格納
            m_biggetSize = m_finalSize.x;
            if (m_biggetSize < m_finalSize.y)
            {
                m_biggetSize = m_finalSize.y;
            }
            if (m_biggetSize < m_finalSize.z)
            {
                m_biggetSize = m_finalSize.z;
            }
        }
    }

    { //座標の設定
        auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue();

        mtx->SetPos(pos_x, pos_y, pos_z);
    }

    { //colliderの座標オフセット値
        auto [offsetpos_x, offsetpos_y, offsetpos_z] = m_offsetPosition.GetValue();

        mtx->SetVec(offsetpos_x, offsetpos_y, offsetpos_z);
    }

    { //colliderの角度オフセット値
        auto [offsetangle_x, offsetangle_y, offsetangle_z] = m_offsetAngle.GetValue();

        mtx->SetAngle(offsetangle_x, offsetangle_y, offsetangle_z);
    }

    mtx->Update();

    //計算ついでに行列をセット
    m_colliderMatrix = mtx->GetMatrix();

    delete mtx;

    m_vecAxisX = {m_colliderMatrix._11, m_colliderMatrix._12, m_colliderMatrix._13};
    m_vecAxisY = {m_colliderMatrix._21, m_colliderMatrix._22, m_colliderMatrix._23};
    m_vecAxisZ = {m_colliderMatrix._31, m_colliderMatrix._32, m_colliderMatrix._33};

    DX11Vec3Normalize(m_vecAxisX, m_vecAxisX);
    DX11Vec3Normalize(m_vecAxisY, m_vecAxisY);
    DX11Vec3Normalize(m_vecAxisZ, m_vecAxisZ);

    if (m_colliderObject != nullptr)
    {
        m_colliderObject->m_transform->SetMatrix(m_colliderMatrix);
        m_colliderObject->m_transform->m_worldPosition.SetValue(m_colliderMatrix._41, m_colliderMatrix._42, m_colliderMatrix._43);
        m_colliderObject->m_transform->m_size.SetValue(m_gameObject->m_transform->m_size.GetValue());
    }
}