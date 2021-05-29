
#include "CCollision2DSystem.h"


#include "../Setup.h"
#include "../ComSystem/ComSystem.h"
#include "../ComSystem/Components/Util/CCollisionBase.h"
#include "../../ThirdParty/ImGui/imgui.h"
#include "../WindowsSystem/CDirectInput.h"
#include "../ImGuiSystem/ImGuiHeaders.h"
#include "../ImGuiSystem/ImGuiHelperFunctions.h"
#include "../Timer/CTimer.h"


std::unordered_map<int, std::unordered_map<E_COLLISION2D_EVENT, std::unordered_map<int, std::function<void(GameObject *)>>>> CCollision2DSystem::m_ListCollisionFunction;

std::unordered_map<int, std::pair<GameObject *, CCollisionBase *>> CCollision2DSystem::m_list2DCollisionObjects;

void CCollision2DSystem::Uninit()
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

void CCollision2DSystem::Update()
{
    //todo 今のは非効率的なのでもっと効率の良い当たり判定の処理を追加
    for (auto itr = m_list2DCollisionObjects.begin(); itr != m_list2DCollisionObjects.end(); ++itr)
    {
        auto itr2 = itr;
        itr2++;
        //最後ではない場合残りと判定をとる
        for (; itr2 != m_list2DCollisionObjects.end(); itr2++)
        {
            auto [obj1, obj1collider] = itr->second;
            auto [obj2, obj2collider] = itr2->second;
            if (CheckCollisionDetection(obj1, obj2, obj1collider, obj2collider))
            {
                if (obj1collider->m_isTrigger.GetValue() || obj2collider->m_isTrigger.GetValue())
                { //Collisionイベント(物理挙動あり)

                    //todo追加すること
                }
                else
                { //Triggerイベント(物理挙動なし)
                    //todo とりあえずSTAYのみ追加なので随時当たり判定追加
                    //同じときのみ当たり判定

                    //当たり判定時の各コンポーネントの対象の関数ポインタをリストから引っ張ってくる
                    RunCollisionDetection(obj1, obj2, E_COLLISION2D_EVENT::TRIGGER_STAY);
                    RunCollisionDetection(obj2, obj1, E_COLLISION2D_EVENT::TRIGGER_STAY);
                }
            }
        }
    }
}

void CCollision2DSystem::EraseCollisionObject(int objid)
{
    //削除対象は存在しているか？
    if (m_list2DCollisionObjects.contains(objid))
    {
        m_list2DCollisionObjects.erase(objid);
    }
}

//private

int CCollision2DSystem::GetObjectID(GameObject *obj)
{
    return obj->m_objID;
}

void CCollision2DSystem::RunCollisionEvent(GameObject *target, GameObject *hitobject, E_COLLISION2D_EVENT type)
{
    auto objid = GetObjectID(target);
    for (auto itr : m_ListCollisionFunction[objid][type])
    {
        itr.second(hitobject);
    }
}

bool CCollision2DSystem::CheckCollisionDetection(GameObject *obj1, GameObject *obj2,
                                                 CCollisionBase *obj1collider, CCollisionBase *obj2collider)
{
    auto obj1trans = obj1->m_transform;
    auto obj2trans = obj2->m_transform;

    //座標取り出し
    auto obj1nowpos = obj1trans->m_worldPosition;
    auto obj2nowpos = obj2trans->m_worldPosition;

    //当たり判定に移動後の座標を使用するか否か
    if (obj1collider->m_isTrigger.GetValue())
    {
        obj1nowpos += obj1trans->m_vector;
    }
    if (obj2collider->m_isTrigger.GetValue())
    {
        obj2nowpos += obj2trans->m_vector;
    }

    auto [nowpos1_x, nowpos1_y, nowpos1_z] = obj1nowpos.GetValue();
    auto [nowpos2_x, nowpos2_y, nowpos2_z] = obj2nowpos.GetValue();

    //todo もし当たり判定の中心を移動させる処理がいるならここで

    auto [size1_x, size1_y, size1_z] = obj1trans->m_size.GetValue();
    auto [size2_x, size2_y, size2_z] = obj2trans->m_size.GetValue();

    //todo当たり判定の大きさを変える処理等はここで

    //これの左、右、上、下、手前、奥の順番で判定をとる
    if (nowpos1_x - size1_x < nowpos2_x + size2_x &&
        nowpos1_x + size1_x > nowpos2_x - size2_x &&

        nowpos1_y - size1_y < nowpos2_y + size2_y &&
        nowpos1_y + size1_y > nowpos2_y - size2_y)
    {
        obj1collider->m_state = true;
        obj2collider->m_state = true;
        return true;
    }
    return false;
}

void CCollision2DSystem::RunCollisionDetection(GameObject *obj1, GameObject *obj2, E_COLLISION2D_EVENT type)
{
    for (auto itr : m_ListCollisionFunction[obj1->m_objID][type])
    {
        itr.second(obj2);
    }
}