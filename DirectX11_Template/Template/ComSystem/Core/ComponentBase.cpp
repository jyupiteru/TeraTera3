/**
 * @file ComponentBase.cpp
 * @author jupiter ()
 * @brief ComponentBaseクラスの実装が記載されたcpp
 */

#include "ComponentBase.h"
#include "../../../Library/ImGui/imgui.h"

std::unordered_map<std::type_index, int> ComponentBase::m_listComponentID;

void ComponentBase::ImGui_Draw(unsigned int windowid)
{
    //ImGui_Drawになにも追加していないとき出る

    ImGui::Text("No ImGui_Draw");
    ImGui::Text("Add ImGui_Draw to component!!!");
}