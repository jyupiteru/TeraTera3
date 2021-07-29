/**
 * @file CCollision3DSystem.cpp
 * @author jupiter ()
 * @brief CCollision3DSystemの実装が書かれたcpp
 */
#include "CCollision3DSystem.h"

#include <algorithm>
#include <tuple>

#include "../../../Setup.h"
#include "../../../ComSystem/ComSystem.h"
#include "../../../ComSystem/Components/Util/CCollisionBase.h"
#include "../../../../ThirdParty/ImGui/imgui.h"
#include "../../../System/CDirectInput.h"
#include "../../../Managers/ImGuiSystem/ImGuiHeaders.h"
#include "../../../Managers/ImGuiSystem/ImGuiHelperFunctions.h"
#include "../../../Timer/CTimer.h"

CCollision3DSystem *CCollision3DSystem::m_instance = nullptr;

using namespace DirectX;

CCollision3DSystem::~CCollision3DSystem()
{
    Uninit();
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::Uninit()
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

    m_listObject.clear();

    for (auto itr = m_list3DCollisionObjects.begin(); itr != m_list3DCollisionObjects.end();)
    {
        itr->second.clear();
    }
    m_list3DCollisionObjects.clear();
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::Update()
{
    //各オブジェクトの衝突回数を制限するためのカウンタ
    int collisioncounter = 0;

    m_collisionCounter = 0;

    m_collisionTime = CTimer::GetInstance().GetProgressTime();

    //辺りをとる際の比率 √2がちょうどいい気もする
    const float colliderjudgerate = 1.45f;

    //検索中にそれぞれのリスト(しかもソート済み)が必要になるのでvectorではなく挿入時にソート済みになるmultimapを使用
    //x、＜y,z,最大の半径,id、部位名＞の順番
    std::vector<std::pair<float, std::tuple<float, float, float, int, std::string>>> list_x;

    //id - 部位名の順番
    std::vector<std::pair<int, std::string>> list_id;

    for (auto &itr : m_list3DCollisionObjects)
    {
        //このオブジェクトは有効か?
        if (m_listObject[itr.first]->m_activeFlag.GetValue())
        {
            //それぞれの部位を登録
            for (auto &itr2 : itr.second)
            {
                float halfsize = itr2.second->m_biggetSize * 0.5f;

                //回転していた場合に取りうる最小の半径？
                halfsize *= colliderjudgerate;

                //各当たり判定の一番小さい(座標的に左手前？)のxの大きさを格納する
                list_x.push_back(std::make_pair(itr2.second->m_colliderMatrix._41 - halfsize,
                                                std::make_tuple(itr2.second->m_colliderMatrix._42 - halfsize,
                                                                itr2.second->m_colliderMatrix._43 - halfsize,
                                                                halfsize,
                                                                itr.first,
                                                                itr2.first)));
            }
        }
    }

    sort(list_x.begin(), list_x.end());

    //Sweep and Pruneの簡易実装(たぶん)
    //整列 => 左手前↓から順に衝突判定

    //todo 少し処理の軽減をしたがかなり重い、なので何かしらの対策する！
    for (auto itr = list_x.begin(); itr != list_x.end(); ++itr)
    {
        auto obj1_x = itr->first;
        auto [obj1_y, obj1_z, obj1_halfsize, obj1_id, obj1_collidername] = itr->second;

        auto itr2 = itr;
        ++itr2;
        if (itr2 == list_x.end())
        { //itrの次がend==次が存在しない(エラーなので)
            break;
        }

        //obj1のx軸の推定範囲にオブジェクトが1個以上あるかどうかある(xのリストはすでにソートしてあるので次の判定のみでOK)
        if (obj1_x + (obj1_halfsize * 2) > itr2->first)
        {
            //obj1のx範囲にあるものをリストに登録
            for (; itr2 != list_x.end(); ++itr2)
            {
                float obj2_x = itr->first;
                auto [obj2_y, obj2_z, obj2_halfsize, obj2_id, obj2_collidername] = itr2->second;

                //範囲外以外を登録
                //本来ならy軸のソートも行うらしいがfor文の増加、DebugじのFPS低下につながるのでここで全処理を行います(releaseはFPS低下なぜかしない)
                if ( //x軸の条件
                    obj1_x + (obj1_halfsize * 2) > obj2_x &&

                    //y軸の条件
                    obj1_y + (obj1_halfsize * 2) > obj2_y &&
                    obj1_y <= obj2_y + obj2_halfsize * 2 &&

                    //z軸の条件
                    obj1_z + (obj1_halfsize * 2) > obj2_z &&
                    obj1_z < obj2_z + obj2_halfsize * 2 &&

                    //同一のオブジェクトからの登録ではないという条件(ないとおそらくAssimp系動くだけで止まる)
                    obj1_id != obj2_id)
                {
                    list_id.push_back(std::make_pair(obj2_id, obj2_collidername));
                }
                else if (obj1_x + (obj1_halfsize * 2) <= obj2_x)
                {
                    //obj1_xの範囲内に存在するであろうobj2_xはもう存在しないので
                    break;
                }
            }

            for (auto &itr_id : list_id)
            {
                //設定した衝突判定の上限内であるか
                if (collisioncounter < MAX_COLLISIONNUM)
                {
                    CCollisionBase *obj1collider = m_list3DCollisionObjects[obj1_id][obj1_collidername];
                    CCollisionBase *obj2collider = m_list3DCollisionObjects[itr_id.first][itr_id.second];
                    if (CheckCollisionDetection(*obj1collider, *obj2collider))
                    {
                        bool judge1 = obj1collider->m_isTrigger.GetValue();
                        bool judge2 = obj2collider->m_isTrigger.GetValue();

                        if (!judge1 && !judge2)
                        { //Collisionイベント(貫通なし) 両方貫通しないオブジェクト

                            //todo CollisionStar3Dが未完成なので凍結 解凍すること

                            //vectorで動いていた場合めり込んでるかもしれないので戻す
                            // obj1->m_transform->m_vector *= -1;
                            // obj2->m_transform->m_vector *= -1;
                            // obj1->m_transform->Update();
                            // obj2->m_transform->Update();
                            // obj1->m_transform->m_vector *= -1;
                            // obj2->m_transform->m_vector *= -1;

                            // RunCollisionDetection(obj1, obj2, E_COLLISION3D_EVENT::COLLISION_STAY);
                            // RunCollisionDetection(obj2, obj1, E_COLLISION3D_EVENT::COLLISION_STAY);
                        }
                        else if (judge1 || judge2)
                        { //Triggerイベント(貫通あり)
                            //todo とりあえずSTAYのみ追加なので随時当たり判定追加
                            //同じときのみ当たり判定

                            //当たり判定時の各コンポーネントの対象の関数ポインタをリストから引っ張ってくる
                            RunCollisionDetection(obj1_id, itr_id.first, E_COLLISION3D_EVENT::TRIGGER_STAY);
                            RunCollisionDetection(itr_id.first, obj1_id, E_COLLISION3D_EVENT::TRIGGER_STAY);
                        }
                        ++collisioncounter;

                        m_collisionCounter++;
                    }
                }
                else
                { //これ以上のものはいない(一定範囲外である)or当たり判定の上限に達した
                    m_reachMax++;
                    break;
                }
            }

            collisioncounter = 0;
            list_id.clear();
        }
    }

    m_collisionTime = CTimer::GetInstance().GetProgressTime() - m_collisionTime;

    list_x.clear();
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::Create()
{
    m_instance = new CCollision3DSystem();
}

void CCollision3DSystem::Delete(bool _flag)
{
    if (_flag)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

CCollision3DSystem &CCollision3DSystem::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::ImGuiDraw(unsigned int windowid)
{
    if (ImGui::TreeNode("CCollision3DSystem"))
    {
        ImGuiDrawDetails(windowid);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("CCollisionObjects"))
    {
        ImGuiDrawCollisionObjects(windowid);
        ImGui::TreePop();
    }
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::ImGuiDrawDetails(unsigned int windowid)
{
    ImGui::BulletText("CollisionTotal : %u", m_collisionCounter);
    ImGui::BulletText("CollisionTime : %0.7f", m_collisionTime);
    ImGui::BulletText("ReachToMaxNum : %u", m_reachMax);
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::ImGuiDrawCollisionObjects(unsigned int windowid)
{

    //std::string hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "hideobject");

    ////何も選択されたことがないのなら何もなくす
    //if (hide_objectname == "None")
    //{
    //    hide_objectname = "";
    //}

    ////強制的にキャスト(エラー出るかも)
    //auto input_hidename = (char *)hide_objectname.c_str();

    ////描画時のフィルター用 TABを押しながら打っていることを確認
    //struct TextFilters
    //{
    //    static int FilterImGuiLetters(ImGuiInputTextCallbackData *data)
    //    {
    //        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_TAB))
    //            return 0;
    //        return 1;
    //    }
    //};

    ////テキストを取得する
    //ImGui::InputText("filter", input_hidename, sizeof(input_hidename),
    //                 ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);

    ////次のものを同じ行に描画する?
    //ImGui::SameLine();
    //HelpMarker((const char *)u8"検索したい衝突対象のオブジェクト名を入れると絞り込み可能、TABを押しながら打つこと");

    ////登録と取得のし直し
    //CImGuiHelper::SetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "hideobject", input_hidename);
    //hide_objectname = CImGuiHelper::GetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "hideobject");

    //std::string selectobject = CImGuiHelper::GetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "selectobject");

    //int selectobjid = -1;

    //if (ImGui::BeginListBox("Collision Objects"))
    //{
    //    //全オブジェクトぶん回し
    //    for (auto &itr : m_list3DCollisionObjects)
    //    {
    //        std::string objname = itr.second.first->m_objectName;

    //        //選択されているものか 同じならtrue
    //        const bool is_selected = (selectobject == objname);

    //        //隠すフラグがたっていないかor絞り込みは機能していないか
    //        if (objname.find(hide_objectname) != std::string::npos || hide_objectname == "")
    //        {
    //            //1コメの引数が表示する内容、２個目が選択されたか？
    //            if (ImGui::Selectable(objname.c_str(), is_selected))
    //            {
    //                selectobject = objname;
    //                //登録内容の更新
    //                CImGuiHelper::SetWindowDisplayContent(windowid, "collisionsystem3d_imgui", "selectobject", objname);
    //            }

    //            //選択されたか
    //            if (is_selected)
    //            {
    //                //選択されている場所をデフォルトにする
    //                ImGui::SetItemDefaultFocus();
    //                selectobjid = itr.first;
    //            }
    //        }
    //    }
    //    ImGui::EndListBox();
    //}

    ////すでに何か選択しているか(対象は存在するか)
    //if (m_list3DCollisionObjects.contains(selectobjid))
    //{
    //    ImGuiDrawCollisionObjectDetails(windowid, selectobjid);
    //}
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::ImGuiDrawCollisionObjectDetails(unsigned int windowid, int objid)
{
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::EraseCollisionObject(int objid)
{
    //そのオブジェクトは登録してあるか
    if (m_list3DCollisionObjects.contains(objid))
    {
        m_list3DCollisionObjects.erase(objid);
    }

    m_listObject.erase(objid);

    //残っていないか
    if (m_ListCollisionFunction.contains(objid))
    {
        for (auto itr = m_ListCollisionFunction[objid].begin(); itr != m_ListCollisionFunction[objid].end();)
        {
            itr->second.clear();
            itr = m_ListCollisionFunction[objid].erase(itr);
        }
    }
}

//================================================================================================
//================================================================================================
//private

int CCollision3DSystem::GetObjectID(GameObject *obj)
{
    return obj->m_objID;
}

//================================================================================================
//================================================================================================

bool CCollision3DSystem::CheckCollisionDetection(const CCollisionBase &obj1collider, const CCollisionBase &obj2collider)
{
    bool sts = false;

    //両方同じ種類の当たり判定である
    if (obj1collider.m_typeCollision == obj2collider.m_typeCollision)
    {
        switch (obj1collider.m_typeCollision)
        {
        case E_TYPE_COLLISIONCLASS::BOX: //両方BOX
            sts = Collision_BoxAndBox(obj1collider, obj2collider);
            break;
        case E_TYPE_COLLISIONCLASS::SPHERE:
            break;
        }
    }
    return sts;
}

//================================================================================================
//================================================================================================

void CCollision3DSystem::RunCollisionDetection(int obj1id, int obj2id, E_COLLISION3D_EVENT type)
{
    GameObject *obj1 = m_listObject[obj1id];
    GameObject *obj2 = m_listObject[obj2id];

    for (auto itr : m_ListCollisionFunction[obj1->m_objID][type])
    {

        //TODO ここでコンポーネントを参照してコンポーネントは生きているか確認する処理がいる
        itr.second(obj2);
    }
}

//================================================================================================
//================================================================================================

bool CCollision3DSystem::Collision_BoxAndBox(const CCollisionBase &obj1collider, const CCollisionBase &obj2collider)
{
    XMFLOAT3 vecSeparate;

    //オブジェクトの距離を計算（ベクトル）
    XMFLOAT3 vecDistance;
    vecDistance.x = obj2collider.m_colliderMatrix._41 - obj1collider.m_colliderMatrix._41;
    vecDistance.y = obj2collider.m_colliderMatrix._42 - obj1collider.m_colliderMatrix._42;
    vecDistance.z = obj2collider.m_colliderMatrix._43 - obj1collider.m_colliderMatrix._43;

    bool sts;

    //正規化後の値を渡すこと
    XMFLOAT3 OBB1Axis[3] = {
        obj1collider.m_vecAxisX,
        obj1collider.m_vecAxisY,
        obj1collider.m_vecAxisZ,
    };

    XMFLOAT3 OBB2Axis[3] = {
        obj2collider.m_vecAxisX,
        obj2collider.m_vecAxisY,
        obj2collider.m_vecAxisZ,
    };

    // OBB-Aの３軸を分離軸にしてチェックしている
    for (int i = 0; i < 3; i++)
    {
        vecSeparate.x = OBB1Axis[i].x;
        vecSeparate.y = OBB1Axis[i].y;
        vecSeparate.z = OBB1Axis[i].z;

        DX11Vec3Normalize(vecSeparate, vecSeparate);

        sts = CompareLength(obj1collider,
                            obj2collider,
                            vecSeparate,
                            vecDistance);
        if (!sts)
        {
            return false;
        }
    }

    // OBB-Bの３軸を分離軸にしてチェックしている
    for (int i = 0; i < 3; i++)
    {
        vecSeparate.x = OBB2Axis[i].x;
        vecSeparate.y = OBB2Axis[i].y;
        vecSeparate.z = OBB2Axis[i].z;

        DX11Vec3Normalize(vecSeparate, vecSeparate);

        sts = CompareLength(obj1collider,
                            obj2collider,
                            vecSeparate,
                            vecDistance);

        if (!sts)
        {
            return false;
        }
    }

    for (int p1 = 0; p1 < 3; p1++)
    {
        for (int p2 = 0; p2 < 3; p2++)
        {
            XMFLOAT3 crossseparate; // 外積ベクトル

            // 外積を計算する
            DX11Vec3Cross(crossseparate, OBB1Axis[p1], OBB2Axis[p2]);
            DX11Vec3Normalize(crossseparate, crossseparate); // 正規化

            sts = CompareLength(
                obj1collider,
                obj2collider,
                crossseparate,
                vecDistance);
            if (sts == false)
            {
                return false;
            }
        }
    }

    return true;
}

//================================================================================================
//================================================================================================

bool CCollision3DSystem::Collision_SphereAndSphere(CCollisionBase *obj1collider, CCollisionBase *obj2collider)
{
    return false;
}

//================================================================================================
//================================================================================================

bool CCollision3DSystem::CompareLength(const CCollisionBase &collider1, const CCollisionBase &collider2,
                                       const XMFLOAT3 &vecSeparate, const XMFLOAT3 &vecDistance)
{
    float fDistance;

    DX11Vec3Dot(fDistance, vecDistance, vecSeparate);
    fDistance = fabsf(fDistance);

    float fShadowA = 0;

    float fShadowB = 0;

    float fShadowAx;
    float fShadowAy;
    float fShadowAz;

    DX11Vec3Dot(fShadowAx, vecSeparate, collider1.m_vecAxisX);
    fShadowAx = fabsf(fShadowAx * (collider1.m_finalSize.x / 2));

    DX11Vec3Dot(fShadowAy, vecSeparate, collider1.m_vecAxisY);
    fShadowAy = fabsf(fShadowAy * (collider1.m_finalSize.y / 2));

    DX11Vec3Dot(fShadowAz, vecSeparate, collider1.m_vecAxisZ);
    fShadowAz = fabsf(fShadowAz * (collider1.m_finalSize.z / 2));

    fShadowA = fShadowAx + fShadowAy + fShadowAz;

    float fShadowBx;
    float fShadowBy;
    float fShadowBz;

    DX11Vec3Dot(fShadowBx, vecSeparate, collider2.m_vecAxisX);
    fShadowBx = fabsf(fShadowBx * (collider2.m_finalSize.x / 2));

    DX11Vec3Dot(fShadowBy, vecSeparate, collider2.m_vecAxisY);
    fShadowBy = fabsf(fShadowBy * (collider2.m_finalSize.y / 2));

    DX11Vec3Dot(fShadowBz, vecSeparate, collider2.m_vecAxisZ);
    fShadowBz = fabsf(fShadowBz * (collider2.m_finalSize.z / 2));

    fShadowB = fShadowBx + fShadowBy + fShadowBz;

    if (fDistance > fShadowA + fShadowB)
    {
        return false;
    }

    return true;
}