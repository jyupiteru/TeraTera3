
#include "CCollisionSystemBase.h"

#include "../../../ThirdParty/ImGui/imgui.h"
#include "../../WindowsSystem/CDirectInput.h"
#include "../../ImGuiSystem/ImGuiHeaders.h"
#include "../../ImGuiSystem/ImGuiHelperFunctions.h"
#include "../../ComSystem/Core/GameObject.h"

using namespace DirectX;

void CCollisionSystemBase::Uninit()
{
    //ID-当たり判定
    for (auto itr = m_ListCollisionFunction.begin(); itr != m_ListCollisionFunction.end();)
    {
        //当たり判定の種類-コンポーネントID
        for (auto itr2 = itr->second.begin(); itr2 != itr->second.end();)
        {
            itr2->second.clear();
            itr2 = itr->second.erase(itr2);
        }
        itr->second.clear();
        itr = m_ListCollisionFunction.erase(itr);
    }
    m_ListCollisionFunction.clear();
}

//================================================================================================
//================================================================================================

void CCollisionSystemBase::ImGuiDraw(unsigned int windowid)
{
    if (ImGui::TreeNode("CCollisionSystemBase"))
    {
        ImGuiDraw_Details(windowid);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("CCollisionObjects"))
    {
        ImGuiDraw_CollisionObjects(windowid);
        ImGui::TreePop();
    }
}

//================================================================================================
//================================================================================================

void CCollisionSystemBase::ImGuiDraw_Details(unsigned int windowid)
{
    ImGui::BulletText("CollisionTotal : %u", m_collisionCounter);
    ImGui::BulletText("CollisionTime : %0.7f", m_collisionTime);
    ImGui::BulletText("ReachToMaxNum : %u", m_reachMax);
}

//================================================================================================
//================================================================================================

void CCollisionSystemBase::ImGuiDraw_CollisionObjects(unsigned int windowid)
{

    std::string hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "hideobject");

    //何も選択されたことがないのなら何もなくす
    if (hide_objectname == "None")
    {
        hide_objectname = "";
    }

    //強制的にキャスト(エラー出るかも)
    auto input_hidename = (char *)hide_objectname.c_str();

    //描画時のフィルター用 TABを押しながら打っていることを確認
    struct TextFilters
    {
        static int FilterImGuiLetters(ImGuiInputTextCallbackData *data)
        {
            if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_TAB))
                return 0;
            return 1;
        }
    };

    //テキストを取得する
    ImGui::InputText("filter", input_hidename, sizeof(input_hidename),
                     ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);

    //次のものを同じ行に描画する?
    ImGui::SameLine();
    HelpMarker((const char *)u8"検索したい衝突対象のオブジェクト名を入れると絞り込み可能、TABを押しながら打つこと");

    //登録と取得のし直し
    CImGuiHelper::SetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "hideobject", input_hidename);
    hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "hideobject");

    std::string selectobject = CImGuiHelper::GetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "selectobject");

    int selectobjid = -1;

    if (ImGui::BeginListBox("Collision Objects"))
    {
        //全オブジェクトぶん回し
        for (auto &itr : m_ListCollisionFunction)
        {
            std::string objname = m_listCollisionObjects[itr.first].first->m_objectName;

            //選択されているものか 同じならtrue
            const bool is_selected = (selectobject == objname);

            //隠すフラグがたっていないかor絞り込みは機能していないか
            if (objname.find(hide_objectname) != std::string::npos || hide_objectname == "")
            {
                //1コメの引数が表示する内容、２個目が選択されたか？
                if (ImGui::Selectable(objname.c_str(), is_selected))
                {
                    selectobject = objname;
                    //登録内容の更新
                    CImGuiHelper::SetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "selectobject", objname);
                }

                //選択されたか
                if (is_selected)
                {
                    //選択されている場所をデフォルトにする
                    ImGui::SetItemDefaultFocus();
                    selectobjid = itr.first;
                }
            }
        }
        ImGui::EndListBox();
    }

    //すでに何か選択しているか(対象は存在するか)
    if (m_listCollisionObjects.contains(selectobjid))
    {
        ImGuiDraw_CollisionObjectDetails(windowid, selectobjid);
    }
}

//================================================================================================
//================================================================================================

void CCollisionSystemBase::ImGuiDraw_CollisionObjectDetails(unsigned int windowid, int objid)
{
}
