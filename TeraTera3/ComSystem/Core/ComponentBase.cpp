/**
 * @file ComponentBase.cpp
 * @author jupiter ()
 * @brief ComponentBaseクラスの実装が記載されたcpp
 */

#include "ComponentBase.h"
#include "../../../ThirdParty/ImGui/imgui.h"

std::unordered_map<std::type_index, int> ComponentBase::m_listComponentID;

void ComponentBase::ImGuiDraw(unsigned int windowid)
{
    //ImGuiDrawになにも追加していないとき出る

    ImGui::Text("No ImGuiDraw");
    ImGui::Text("Add ImGuiDraw to component!!!");
}