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
#include "../../Behavior/ComLightPoint/ComLightPoint.h"

void ComLight::Init()
{
    // コンスタントバッファ作成
    bool sts = CreateConstantBuffer(
        CDirectXGraphics::GetInstance().GetDXDevice(), // デバイス
        sizeof(tagConstantBufferLight),                // サイズ
        &m_pConstantBufferLight);                      // コンスタントバッファ４
    if (!sts)
    {
        MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
    }

    m_gameObject->m_transform->m_color.SetValue(255.0f, 255.0f, 0.0f, 0.9f);

    m_ambientColor.SetValue(100.0f, 100.0f, 100.0f);

    m_directionalColor.SetValue(255.0f, 255.0f, 255.0f);
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
    auto [ambient_x, ambient_y, ambient_z] = m_ambientColor.GetValue();
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
    tagConstantBufferLight cb;

    { //ディレクションライト
        //方向情報を格納して正規化する
        std::tie(cb.directionalLight.directionalLightDirection.x,
                 cb.directionalLight.directionalLightDirection.y,
                 cb.directionalLight.directionalLightDirection.z) = m_lightDirection.GetValue();
        DX11Vec3Normalize(cb.directionalLight.directionalLightDirection, cb.directionalLight.directionalLightDirection);

        //ディレクショナルライトの色情報を格納
        std::tie(cb.directionalLight.directionalLightColor.x,
                 cb.directionalLight.directionalLightColor.y,
                 cb.directionalLight.directionalLightColor.z) = m_directionalColor.GetValue();

        cb.directionalLight.directionalLightColor.x /= 256.0f;
        cb.directionalLight.directionalLightColor.y /= 256.0f;
        cb.directionalLight.directionalLightColor.z /= 256.0f;

        cb.directionalLight.directionalLightColor.w = 0.0f;
    }

    { //ポイントライト
        bool pointflag = true;

        //ポイントライトは生成済みか？
        if (auto pointlight = ComLightPoint::GetInstance(); pointlight != nullptr)
        {
            //コンポーネントと所持しているオブジェクトはアクティブか?
            if (pointlight->m_enable.GetValue() == true &&
                pointlight->m_gameObject->m_activeFlag.GetValue() == true)
            {
                //ポイントライトの色を取得
                std::tie(cb.pointLight.pointLightColor.x,
                         cb.pointLight.pointLightColor.y,
                         cb.pointLight.pointLightColor.z) = pointlight->m_lightColor.GetValue();

                cb.pointLight.pointLightColor.x /= 256.0f;
                cb.pointLight.pointLightColor.y /= 256.0f;
                cb.pointLight.pointLightColor.z /= 256.0f;
                cb.pointLight.pointLightColor.w = 0.0f;

                //ポイントライトの座標を取得
                std::tie(cb.pointLight.pointLightPosition.x,
                         cb.pointLight.pointLightPosition.y,
                         cb.pointLight.pointLightPosition.z) = pointlight->m_gameObject->m_transform->m_worldPosition.GetValue();

                cb.pointLight.pointLightPosition.w = 0.0f;

                //ポイントライトの影響範囲を計算
                cb.pointLight.pointLightRange = pointlight->m_range.GetValue();
                pointflag = false;
            }
        }

        //ポイントライトの値は設定されていないか?
        if (pointflag == true)
        {
            cb.pointLight.pointLightColor = { 0.0f, 0.0f, 0.0f,0.0f };
            cb.pointLight.pointLightPosition = { 0.0f, 0.0f, 0.0f,0.0f };
            cb.pointLight.pointLightRange = 0.0f;
        }
    }

    { //共通処理
        std::tie(cb.eyePos.x,
                 cb.eyePos.y,
                 cb.eyePos.z) = m_pComCamera->m_cEyePos.GetValue();

        //環境光を取り出し値を0.0f~1.0fに収めセット
        std::tie(cb.ambient.x,
                 cb.ambient.y,
                 cb.ambient.z) = m_ambientColor.GetValue();

        cb.ambient.x /= 256.0f;
        cb.ambient.y /= 256.0f;
        cb.ambient.z /= 256.0f;

        cb.ambient.w = 0.0f;
    }

    cb.directionalLight.pad = 0.0f;
    cb.pointLight.pad = { 0.0f,0.0f,0.0f };
    cb.pad3 = 0.0f;

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