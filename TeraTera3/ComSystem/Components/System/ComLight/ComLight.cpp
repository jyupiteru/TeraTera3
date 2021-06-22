/**
 * @file ComLight.cpp
 * @author jupiter
 * @brief ComLightが記述されたcpp
 * @version 1.0
 */
#include "ComLight.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../ImGuiSystem/ImGuiHelperFunctions.h"
#include "../../../Core/GameObject.h"
#include "../../Behavior/ComTransform/ComTransform.h"
#include "../../Shape/ComSphere/ComSphere.h"
#include "../../Behavior/Com3DModelAssimp/Com3DModelAssimp.h"

void ComLight::Init()
{
    m_typeLight = E_TYPE_LIGHT::DIRECTION;

    // コンスタントバッファ作成
    bool sts = CreateConstantBuffer(
        CDirectXGraphics::GetInstance().GetDXDevice(), // デバイス
        sizeof(ConstantBufferLight),                   // サイズ
        &m_pConstantBufferLight);                      // コンスタントバッファ４
    if (!sts)
    {
        MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
    }

    m_gameObject->m_transform->m_color.SetValue(255.0f, 255.0f, 0.0f, 0.9f);

    m_pComSphere = m_gameObject->AddComponent<ComSphere>();
    m_gameObject->m_transform->m_size.SetValue(10.0f, 10.0f, 10.0f);
}

//================================================================================================
//================================================================================================

void ComLight::Uninit()
{
    if (m_pConstantBufferLight)
    {
        m_pConstantBufferLight->Release();
        m_pConstantBufferLight = nullptr;
    }
}

//================================================================================================
//================================================================================================

void ComLight::Ready()
{
    m_pComCamera = GameObject::Find("Camera")->GetComponent<ComCamera>();
}

//================================================================================================
//================================================================================================

void ComLight::Update()
{
    UpdateLight();
}

//================================================================================================
//================================================================================================

void ComLight::ImGuiDraw(unsigned int windowid)
{
    auto [ambient_x, ambient_y, ambient_z] = m_lightColor.GetValue();
    ImGui::BulletText("Ambient");
    ImGui::SameLine();
    HelpMarker((const char *)u8"環境光のこと");
    ImGui::Indent();
    ImGui::Text("X : %d, Y : %d, Z : %d", ambient_x, ambient_y, ambient_z);
}

//================================================================================================
//================================================================================================

void ComLight::UpdateLight()
{
    switch (m_typeLight)
    {
    case E_TYPE_LIGHT::DIRECTION:
        UpdateDirectionLight();
        break;

    case E_TYPE_LIGHT::POINT:
        break;

    case E_TYPE_LIGHT::SPOT:
        break;
    }
}

//================================================================================================
//================================================================================================

void ComLight::UpdateDirectionLight()
{
    ConstantBufferLight cb;

    std::tie(cb.EyePos.x,
             cb.EyePos.y,
             cb.EyePos.z) = m_pComCamera->m_cEyePos.GetValue();

    cb.pad = 0.0;

    //方向情報を格納して正規化する
    std::tie(cb.LightDirection.x,
             cb.LightDirection.y,
             cb.LightDirection.z) = m_lightDirection.GetValue();
    DX11Vec3Normalize(cb.LightDirection, cb.LightDirection);
    cb.pad2 = 0;

    //環境光を取り出し値を0.0f~1.0fに収めセット
    std::tie(cb.Ambient.x,
             cb.Ambient.y,
             cb.Ambient.z) = m_lightColor.GetValue();
    m_gameObject->m_transform->m_color.SetValue(cb.Ambient.x, cb.Ambient.y, cb.Ambient.z, 1.0f);
    cb.Ambient.x /= 256.0f;
    cb.Ambient.y /= 256.0f;
    cb.Ambient.z /= 256.0f;

    //情報を上書き
    CDirectXGraphics::GetInstance().GetImmediateContext()->UpdateSubresource(m_pConstantBufferLight,
                                                                             0,
                                                                             nullptr,
                                                                             &cb,
                                                                             0, 0);

    // コンスタントバッファ4をｂ3レジスタへセット（頂点シェーダー用）
    CDirectXGraphics::GetInstance().GetImmediateContext()->VSSetConstantBuffers(4, 1, &m_pConstantBufferLight);
    // コンスタントバッファ4をｂ3レジスタへセット(ピクセルシェーダー用)
    CDirectXGraphics::GetInstance().GetImmediateContext()->PSSetConstantBuffers(4, 1, &m_pConstantBufferLight);
}