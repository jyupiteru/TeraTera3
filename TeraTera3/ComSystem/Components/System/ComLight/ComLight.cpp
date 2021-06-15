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

void ComLight::Init()
{
    m_type = E_LightType::DIRECTION;

    // コンスタントバッファ作成
    bool sts = CreateConstantBuffer(
        CDirectXGraphics::GetInstance().GetDXDevice(), // デバイス
        sizeof(ConstantBufferLight),                   // サイズ
        &m_pConstantBufferLight);                      // コンスタントバッファ４
    if (!sts)
    {
        MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
    }

    m_pComSphere = m_gameObject->AddComponent<ComSphere>();
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
    m_pComCamera = GameObject::Find("camera")->GetComponent<ComCamera>();
}

//================================================================================================
//================================================================================================

void ComLight::Update()
{
    ConstantBufferLight cb;

    std::tie(cb.EyePos.x,
             cb.EyePos.y,
             cb.EyePos.z) = m_pComCamera->m_cEyePos.GetValue();
    cb.EyePos.w = 1.0;

    std::tie(cb.LightDirection.x,
             cb.LightDirection.y,
             cb.LightDirection.z) = m_gameObject->m_transform->m_worldPosition.GetValue();
    cb.LightDirection.w = 0;

    std::tie(cb.Ambient.x,
             cb.Ambient.y,
             cb.Ambient.z,
             cb.Ambient.w) = m_ambient.GetValue();

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

//================================================================================================
//================================================================================================

void ComLight::ImGuiDraw(unsigned int windowid)
{
    auto [ambient_x, ambient_y, ambient_z, ambient_a] = m_ambient.GetValue();
    ImGui::BulletText("Ambient");
    ImGui::SameLine();
    HelpMarker((const char *)u8"???まだわかってない");
    ImGui::Indent();
    ImGui::Text("X : %d, Y : %d, Z : %d, A : %d", ambient_x, ambient_y, ambient_z, ambient_a);
}