/**
 * @file ComTransform.cpp
 * @author jupiter ()
 * @brief ComTransformの実装を記載したcpp
 */

#include "ComTransform.h"
#include <memory>
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../ImGuiSystem/ImGuiHelperFunctions.h"

using namespace DirectX;

void ComTransform::Init()
{
    m_typeComponent.SetValue(E_TYPE_COMPONENT::SYSTEM);

    m_size.SetValue(1.0f, 1.0f, 1.0f);

    m_color.SetValue(256.0f, 256.0f, 256.0f, 1.0f);

    m_sizeRateSize.SetValue(1.0f);

    m_offsetSize.SetValue(1.0f, 1.0f, 1.0f);

    DX11MtxIdentity(m_matrix); //単位行列化 つまり初期化
    DX11MtxIdentity(m_noScalingMatrix);
}

//================================================================================================
//================================================================================================

void ComTransform::Uninit()
{
}

//================================================================================================
//================================================================================================

void ComTransform::ImGuiDraw(unsigned int windowid)
{
    //ワールド座標の表示
    auto [worldpos_x, worldpos_y, worldpos_z] = m_worldPosition.GetValue();
    ImGui::BulletText("WorldPosition");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", worldpos_x, worldpos_y, worldpos_z);
    ImGui::Unindent();

    //ローカル座標の表示
    auto [localpos_x, localpos_y, localpos_z] = m_localPosition.GetValue();
    ImGui::BulletText("LocalPosition");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", localpos_x, localpos_y, localpos_z);
    ImGui::Unindent();

    //角度の表示
    auto [angle_x, angle_y, angle_z] = m_angle.GetValue();
    ImGui::BulletText("Angle");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", angle_x, angle_y, angle_z);
    ImGui::Unindent();

    //大きさの表示
    auto [size_x, size_y, size_z] = m_size.GetValue();
    ImGui::BulletText("Size");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", size_x, size_y, size_z);
    ImGui::Unindent();

    //移動量の表示
    auto [vector_x, vector_y, vector_z] = m_vector.GetValue();
    ImGui::BulletText("Vector");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", vector_x, vector_y, vector_z);
    ImGui::Unindent();

    //色の表示
    auto [color_x, color_y, color_z, color_w] = m_color.GetValue();
    ImGui::BulletText("Color");
    ImGui::Indent();
    ImGui::Text("R : %0.1f, G : %0.1f, B : %0.1f, a : %0.1f", color_x, color_y, color_z, color_w);
    ImGui::Unindent();

    //オフセットの大きさの表示
    auto [offsize_x, offsize_y, offsize_z] = m_offsetSize.GetValue();
    ImGui::BulletText("OffsetSize");
    ImGui::SameLine();
    HelpMarker((const char *)u8"モデル等の初期サイズ");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", offsize_x, offsize_y, offsize_z);
    ImGui::Unindent();

    //大きさの比率の表示
    ImGui::BulletText("SizeRate : %0.10f", m_sizeRateSize.GetValue());
    ImGui::SameLine();
    HelpMarker((const char *)u8"モデル等の本来の大きさとワールドの大きさ1の比率");
}

//================================================================================================
//================================================================================================

void ComTransform::UpdateMatrix()
{
    //行列は拡大、回転、移動の順番！
    //DX11MtxIdentity(m_matrix);

    XMFLOAT4 localVectorX; //ローカル（モデル等）のXベクトルがワールドでどうなっているかが入る
    XMFLOAT4 localVectorY; //ローカル（モデル等）のYベクトルがワールドでどうなっているかが入る
    XMFLOAT4 localVectorZ; //ローカル（モデル等）のZベクトルがワールドでどうなっているかが入る

    XMFLOAT4X4 size;

    { //行列を一度大きさを元に戻す処理 しないと大きさと角度同時に変更できない
        DX11MtxScale(1.0f / m_lastSize.x, 1.0f / m_lastSize.y, 1.0f / m_lastSize.z, size);
        DX11MtxMultiply(m_matrix, size, m_matrix);
    }

    { //拡大処理
        auto [size_x, size_y, size_z] = m_size.GetValue();

        //1当たりの大きさを取得?
        auto firstSize = m_sizeRateSize.GetValue();
        auto sizerate = 1.0f / firstSize;

        DX11MtxScale(size_x * sizerate, size_y * sizerate, size_z * sizerate, size);

        //最終の大きさの更新
        m_lastSize.x = size_x * sizerate;
        m_lastSize.y = size_y * sizerate;
        m_lastSize.z = size_z * sizerate;
    }

    { //回転の処理

        //ローカルのXベクトル抜き出し作業
        localVectorX.x = m_matrix._11;
        localVectorX.y = m_matrix._12;
        localVectorX.z = m_matrix._13;
        localVectorX.w = 0.0f;

        //ローカルのYベクトル抜き出し作業
        localVectorY.x = m_matrix._21;
        localVectorY.y = m_matrix._22;
        localVectorY.z = m_matrix._23;
        localVectorY.w = 0.0f;

        //ローカルのZベクトル抜き出し作業
        localVectorZ.x = m_matrix._31;
        localVectorZ.y = m_matrix._32;
        localVectorZ.z = m_matrix._33;
        localVectorZ.w = 0.0f;

        //クオータニオン生成
        //todo ここはそのうち別のメンバにして分ける

        XMFLOAT4 quaternion;

        //行列からクォータニオンを生成する
        DX11GetQtfromMatrix(m_matrix, quaternion);

        XMFLOAT4 qtx, qty, qtz;

        //各軸を中心に回転したクォータニオン生成
        auto [angle_x, angle_y, angle_z] = m_angle.GetValue();

        DX11QtRotationAxis(qtx, localVectorX, m_lastAngle.x - angle_x);
        DX11QtRotationAxis(qty, localVectorY, m_lastAngle.y - angle_y);
        DX11QtRotationAxis(qtz, localVectorZ, m_lastAngle.z - angle_z);

        //最終角度の更新
        m_lastAngle.x = angle_x;
        m_lastAngle.y = angle_y;
        m_lastAngle.z = angle_z;

        //クォータニオンを合成
        XMFLOAT4 i1;
        XMFLOAT4 i2;
        XMFLOAT4 i3;

        DX11QtMul(i1, quaternion, qtx);

        DX11QtMul(i2, qty, qtz);

        DX11QtMul(i3, i1, i2);
        //クォータニオンをノーマライズ
        DX11QtNormalize(i3, i3);

        //クォータニオンから行列を生成
        DX11MtxFromQt(m_matrix, i3);
        DX11MtxFromQt(m_noScalingMatrix, i3);
    }

    { //移動量追加 大きさを追加より先に！
        auto [vec_x, vec_y, vec_z] = m_vector.GetValue();
        //ローカルのx方向への移動量追加
        m_worldPosition.AddValue(m_matrix._11 * vec_x,
                                 m_matrix._12 * vec_x,
                                 m_matrix._13 * vec_x);

        m_worldPosition.AddValue(m_matrix._21 * vec_y,
                                 m_matrix._22 * vec_y,
                                 m_matrix._23 * vec_y);

        m_worldPosition.AddValue(m_matrix._31 * vec_z,
                                 m_matrix._32 * vec_z,
                                 m_matrix._33 * vec_z);
    }

    DX11MtxMultiply(m_matrix, size, m_matrix);

    //現在地の追加
    std::tie(m_matrix._41, m_matrix._42, m_matrix._43) = m_worldPosition.GetValue();

    std::tie(m_noScalingMatrix._41, m_noScalingMatrix._42, m_noScalingMatrix._43) = m_worldPosition.GetValue();
}
