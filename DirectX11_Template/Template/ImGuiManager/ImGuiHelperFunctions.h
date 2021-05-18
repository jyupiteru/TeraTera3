/**
 * @file ImGuiHelperFunctions.h
 * @author jupiter ()
 * @brief CImGuiManagerをサポートする関数、クラスを記載しているヘッダ
 */

#include "../../Library/ImGui/imgui.h"

#pragma once

/**
 * @brief コンセプトを使用した指定するやつ
 * @tparam componenttype 判定したいコンポーネント名
 */
template <class componenttype>
concept Has_ImGUiDraw = requires(componenttype *object)
{
    //ImGuiDraw(int型引数)を持っている
    object->ImGuiDraw(0);
};

/**
 * @brief ImGuiでヘルプを表示する際に使用する関数
 * @n imgui_demoよりコピーペーストしてコメント追加しただけ
 * @param desc 表示させたい文字列
 */
static void HelpMarker(const char *desc)
{
    ImGui::TextDisabled("(?)");

    //選択されているかどうか
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}