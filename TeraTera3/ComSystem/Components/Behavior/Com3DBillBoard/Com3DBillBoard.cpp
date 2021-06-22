/**
 * @file Com3DBillBoard.cpp
 * @author jupiter
 * @brief Com3DBillBoardが載っているcpp
 * @date 2020-08-25
 */

#include <DirectXMath.h>

#include "Com3DBillBoard.h"

void Com3DBillBoard::Init()
{
    m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT3D);
    Com3DTexture::Init();
}

//================================================================================================
//================================================================================================

void Com3DBillBoard::Uninit()
{
    Com3DTexture::Uninit();
}

//================================================================================================
//================================================================================================

void Com3DBillBoard::Ready()
{
    Com3DTexture::Ready();
    m_pComCamera = GameObject::Find("Camera")->GetComponent<ComCamera>();
}

//================================================================================================
//================================================================================================

void Com3DBillBoard::Draw()
{
    DirectX::XMFLOAT4X4 cameramat = m_pComCamera->GetEyeMatrix();
    DirectX::XMFLOAT4X4 mat = this->m_gameObject->m_transform->GetMatrix();

    mat._11 = cameramat._11;
    mat._12 = cameramat._21;
    mat._13 = cameramat._31;

    mat._21 = cameramat._12;
    mat._22 = cameramat._22;
    mat._23 = cameramat._32;

    mat._31 = cameramat._13;
    mat._32 = cameramat._23;
    mat._33 = cameramat._33;

    mat._14 = 0;
    mat._24 = 0;
    mat._34 = 0;
    mat._44 = 1;

    DirectX::XMFLOAT4X4 size;

    auto [size_x, size_y, size_z] = m_gameObject->m_transform->m_size.GetValue();

    //拡大処理
    DX11MtxScale(size_x,
                 size_y,
                 size_z, size);

    DX11MtxMultiply(mat, size, mat);

    m_gameObject->m_transform->SetMatrix(mat);

    Com3DTexture::Draw();
}

//================================================================================================
//================================================================================================

void Com3DBillBoard::ImGuiDraw(unsigned int windowid)
{
    Com3DTexture::ImGuiDraw(windowid);
}