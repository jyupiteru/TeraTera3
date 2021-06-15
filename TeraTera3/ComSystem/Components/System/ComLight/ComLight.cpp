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
    m_pComCamera = GameObject::Find("camera")->GetComponent<ComCamera>();
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
    auto [ambient_x, ambient_y, ambient_z, ambient_a] = m_lightColor.GetValue();
    ImGui::BulletText("Ambient");
    ImGui::SameLine();
    HelpMarker((const char *)u8"???まだわかってない");
    ImGui::Indent();
    ImGui::Text("X : %d, Y : %d, Z : %d, A : %d", ambient_x, ambient_y, ambient_z, ambient_a);
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

    //方向情報を格納、正規化はGPU側で行う
    std::tie(cb.LightDirection.x,
             cb.LightDirection.y,
             cb.LightDirection.z) = m_lightDirection.GetValue();

    cb.pad2 = 0;

    std::tie(cb.Ambient.x,
             cb.Ambient.y,
             cb.Ambient.z,
             cb.Ambient.w) = m_lightColor.GetValue();

    //環境色は設定されているか？
    if (cb.Ambient.x == 0 &&
        cb.Ambient.y == 0 &&
        cb.Ambient.z == 0 &&
        cb.Ambient.w == 0)
    {
        //されていないのでわかりやすく表示
        m_gameObject->m_transform->m_color.SetValue(256, 256, 256, 0.9f);
    }
    else
    {
        m_gameObject->m_transform->m_color.SetValue(cb.Ambient.x, cb.Ambient.y, cb.Ambient.z, cb.Ambient.w);
    }

    //情報を上書き？
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