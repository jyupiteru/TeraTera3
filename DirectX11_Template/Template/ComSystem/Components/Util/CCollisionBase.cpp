
#include "CCollisionBase.h"
#include "../../../../Library/ImGui/imgui.h"
#include "../../../ImGuiManager/ImGuiHelperFunctions.h"

void CCollisionBase::ImGui_Draw(unsigned int windowid)
{
    //ローカルのx軸はどの方向に向いているか
    ImGui::BulletText("AxisX");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", m_vecAxisX.x, m_vecAxisX.y, m_vecAxisX.z);
    ImGui::Unindent();

    //ローカルのy軸はどの方向に向いているか
    ImGui::BulletText("AxisY");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", m_vecAxisY.x, m_vecAxisY.y, m_vecAxisY.z);
    ImGui::Unindent();

    //ローカルのz軸はどの方向に向いているか
    ImGui::BulletText("AxisZ");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", m_vecAxisZ.x, m_vecAxisZ.y, m_vecAxisZ.z);
    ImGui::Unindent();

    //前フレームの大きさの表示
    ImGui::BulletText("FinalSize");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", m_finalSize.x, m_finalSize.y, m_finalSize.z);
    ImGui::Unindent();

    //最も大きい大きさの表示
    ImGui::BulletText("BiggestSize : %f", m_biggetSize);

    if (m_draw)
    {
        ImGui::BulletText("isDraw : true");
    }
    else
    {
        ImGui::BulletText("isDraw : false");
    }
    ImGui::SameLine();
    HelpMarker((const char*)u8"衝突範囲を描画するかどうか");

    if (m_isFirstJustSize)
    {
        ImGui::BulletText("isFirstJustSize : true");
    }
    else
    {
        ImGui::BulletText("isFirstJustSize : false");
    }
    ImGui::SameLine();
    HelpMarker((const char *)u8"初期の当たり判定のサイズをモデルと一緒にするかどうか");

    //オフセットの座標の表示
    auto [offpos_x, offpos_y, offpos_z] = m_offsetPosition.GetValue();
    ImGui::BulletText("OffsetPos");
    ImGui::SameLine();
    HelpMarker((const char *)u8"座標の補正値");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", offpos_x, offpos_y, offpos_z);
    ImGui::Unindent();

    //オフセットの角度の表示
    auto [offangle_x, offangle_y, offangle_z] = m_offsetAngle.GetValue();
    ImGui::BulletText("OffsetAngle");
    ImGui::SameLine();
    HelpMarker((const char *)u8"角度の補正値");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", offangle_x, offangle_y, offangle_z);
    ImGui::Unindent();

    //オフセットの大きさの表示
    auto [offsize_x, offsize_y, offsize_z] = m_offsetSize.GetValue();
    ImGui::BulletText("OffsetSize");
    ImGui::SameLine();
    HelpMarker((const char *)u8"大きさの補正値");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f, Z : %0.1f", offsize_x, offsize_y, offsize_z);
    ImGui::Unindent();

    //オフセットの大きさの表示
    auto [color_r, color_g, color_b, color_a] = m_color.GetValue();
    ImGui::BulletText("Color");
    ImGui::Indent();
    ImGui::Text("R : %0.1f, G : %0.1f, B : %0.1f,A : %0.1d", color_r, color_g, color_b, color_a);
    ImGui::Unindent();
}