/**
 * @file ComCamera.cpp
 * @author jupiter
 * @brief ComCameraが記述されたcpp
 * @date 2020-08-22
 */

#include "../../../Core/GameObject.h"
#include "ComCamera.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../ImGuiSystem/ImGuiHelperFunctions.h"

using namespace DirectX;

void ComCamera::Init()
{
    m_range.SetValue(40);
}

//================================================================================================
//================================================================================================

void ComCamera::Ready()
{
    m_cUp.SetValue(0.0f, 1.0f, 0.0f);
}

//================================================================================================
//================================================================================================

void ComCamera::Update()
{

    auto [angle_x, angle_y, angle_z] = m_gameObject->m_transform->m_angle.GetValue();
    auto [firstangle_x, firstangle_y, firstangle_z] = m_firstAngle.GetValue();

    auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue();

    //todo ここカメラ動かす処理を作りmtxはstaticにしても大丈夫か確認して変更
    switch (std::shared_ptr<CMatrix> mtx = std::make_shared<CMatrix>(); m_typeFixed)
    {
    case E_TYPE_FIXED::CAMERA:
        //カメラ座標を中心に移動
        m_cEyePos.SetValue(pos_x, pos_y, pos_z);

        m_cLookPos.SetValue(CreatePos(static_cast<float>(m_range.GetValue()),
                                      angle_x + firstangle_x,
                                      angle_y + firstangle_y));
        m_cLookPos += m_cEyePos;
        break;

    case E_TYPE_FIXED::LOOKAT:
        //注視点の座標を中心に移動
        m_cLookPos.SetValue(pos_x, pos_y, pos_z);

        m_cEyePos.SetValue(CreatePos(static_cast<float>(m_range.GetValue()),
                                     angle_x + firstangle_x,
                                     angle_y + firstangle_y));
        m_cEyePos += m_cLookPos;
        break;

        //他から座標を取り込む必要のある処理 or その他
    default:

        //座標は取り込んであるか
        if (m_otherTransform != nullptr)
        {
            auto [otherpos_x, otherpos_y, otherpos_z] = m_otherTransform->m_worldPosition.GetValue();
            auto [otherangle_x, otherangle_y, otherangle_z] = m_otherTransform->m_angle.GetValue();
            auto [offpos_x, offpos_y, offpos_z] = m_offsetPos.GetValue();

            //行列をセット(たぶんwがいる？)
            mtx->SetMatrix(m_otherTransform->GetMatrix());

            //対象への座標の移動とオフセット値の追加
            mtx->SetPos(otherpos_x, otherpos_y, otherpos_z);
            mtx->SetVec(offpos_x, offpos_y, offpos_y);

            //角度の追加
            if (m_typeFixed == E_TYPE_FIXED::MODELCAMERA)
            {
                mtx->SetAngle(angle_x + firstangle_x,
                              -angle_y + firstangle_y,
                              angle_z + firstangle_z);
            }
            else
            {
                mtx->SetAngle(-angle_x + firstangle_x,
                              angle_y + firstangle_y,
                              angle_z + firstangle_z);
            }
            mtx->Update();

            auto matrix = mtx->GetMatrix();
            int range = m_range.GetValue();
            m_cUp.SetValue(matrix._21, matrix._22, matrix._23);

            switch (m_typeFixed)
            {
            case E_TYPE_FIXED::MODELCAMERA:
                //モデル座標ありのカメラ中心回転

                m_cEyePos.SetValue(matrix._41, matrix._42, matrix._43);

                m_cLookPos.SetValue(matrix._41 + matrix._31 * range,
                                    matrix._42 + matrix._32 * range,
                                    matrix._43 + matrix._33 * range);
                break;

            case E_TYPE_FIXED::MODELLOOKAT:
                //モデル座標ありの注視点中心回転
                m_cLookPos.SetValue(matrix._41, matrix._42, matrix._43);

                //ここから下が上の処理と違う
                m_cEyePos.SetValue(matrix._41 - matrix._31 * range,
                                   matrix._42 - matrix._32 * range,
                                   matrix._43 - matrix._33 * range);
                break;

            case E_TYPE_FIXED::SETBOTH:
            default:
                //座標は両方指定する場合
                break;
            }
            m_gameObject->m_transform->m_worldPosition.SetValue(matrix._41, matrix._42, matrix._43);
        }
    }
    CreateViewMatrix();
}

//================================================================================================
//================================================================================================

void ComCamera::ImGuiDraw(unsigned int windowid)
{
    auto [eyepos_x, eyepos_y, eyepos_z] = m_cEyePos.GetValue();
    ImGui::BulletText("EyePosition");
    ImGui::Indent();
    ImGui::Text("X : %0.1f%, Y : %0.1f%, Z : %0.1f%", eyepos_x, eyepos_y, eyepos_z);
    ImGui::Unindent();

    auto [lookpos_x, lookpos_y, lookpos_z] = m_cLookPos.GetValue();
    ImGui::BulletText("LookPosition");
    ImGui::Indent();
    ImGui::Text("X : %0.1f%, Y : %0.1f%, Z : %0.1f%", lookpos_x, lookpos_y, lookpos_z);
    ImGui::Unindent();

    auto [up_x, up_y, up_z] = m_cUp.GetValue();
    ImGui::BulletText("Up");
    ImGui::Indent();
    ImGui::Text("X : %0.1f%, Y : %0.1f%, Z : %0.1f%", up_x, up_y, up_z);
    ImGui::Unindent();

    ImGui::BulletText("Range : %d", m_range.GetValue());

    auto [offpos_x, offpos_y, offpos_z] = m_offsetPos.GetValue();
    ImGui::BulletText("OffsetPosition");
    ImGui::Indent();
    ImGui::Text("X : %0.1f%, Y : %0.1f%, Z : %0.1f%", offpos_x, offpos_y, offpos_z);
    ImGui::Unindent();

    auto [firstangle_x, firstangle_y, firstangle_z] = m_firstAngle.GetValue();
    ImGui::BulletText("FirstAngle");
    ImGui::Indent();
    ImGui::Text("X : %0.1f%, Y : %0.1f%, Z : %0.1f%", firstangle_x, firstangle_y, firstangle_z);
    ImGui::Unindent();

    if (m_otherTransform != nullptr)
    {
        ImGui::BulletText("Reference Object : %s", m_otherTransform->m_gameObject->m_objectName.c_str());
    }
    else
    {
        ImGui::BulletText("Reference Object : None");
    }
}

//================================================================================================
//================================================================================================

void ComCamera::GetPLANEForFrustum(tagPLANE *plane)
{
    DirectX::XMFLOAT4X4 frustummatrix;

    //フラスタム平面を取り出すための行列を作成
    DX11MtxMultiply(frustummatrix, m_view, m_projection);

    //フラスタム平面の
    GetPlanefromProjectionmatrixWithNormalize(plane, frustummatrix);
}

//================================================================================================
//================================================================================================

void ComCamera::CreateViewMatrix()
{
    auto [eye_x, eye_y, eye_z] = m_cEyePos.GetValue();
    auto [look_x, look_y, look_z] = m_cLookPos.GetValue();
    auto [upx, up_y, up_z] = m_cUp.GetValue();

    ALIGN16 XMVECTOR Eye = XMVectorSet(eye_x,
                                       eye_y,
                                       eye_z, 0.0f);

    ALIGN16 XMVECTOR At = XMVectorSet(look_x,
                                      look_y,
                                      look_z, 0.0f);

    ALIGN16 XMVECTOR Up = XMVectorSet(upx,
                                      up_y,
                                      up_z, 0.0f);

    ALIGN16 XMMATRIX camera;

    camera = XMMatrixLookAtLH(Eye, At, Up);

    XMStoreFloat4x4(&m_view, camera);
    DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, m_view);
}

//================================================================================================
//================================================================================================

void ComCamera::CreateProjectionMatrix()
{
    ALIGN16 XMMATRIX projection;

    projection = XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_near, m_far);

    XMStoreFloat4x4(&m_projection, projection);
    DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, m_projection);
}

//================================================================================================
//================================================================================================

std::tuple<float, float, float> ComCamera::CreatePos(float radius, float elevation, float azimuth)
{
    elevation -= 90;
    azimuth += 90;
    //ラジアンに変換
    elevation = ToRad(elevation);
    azimuth = ToRad(azimuth);

    //仰角に応じた半径を変換
    float r = radius * sinf(elevation);

    //視覚座標を計算(3D座標をデカルト座標へ変換)
    float pos_x = r * cosf(azimuth);
    float posy = radius * cosf(elevation);
    float pos_z = r * sinf(azimuth);

    float elevationplus90 = elevation + ToRad(90.0); //仰角を90度足す

    r = radius * sinf(elevationplus90);

    m_cUp.SetValue(r * cosf(azimuth), radius * cosf(elevationplus90), r * sinf(azimuth));

    return {pos_x, posy, pos_z};
}