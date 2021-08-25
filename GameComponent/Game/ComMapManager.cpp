/**
 * @file ComMapManager.cpp
 * @author jupiter ()
 * @brief ComMapManagerの実装が記述されたcpp
 */

#include "ComMapManager.h"
#include "../ComDataManager/ComDataManager.h"
#include "ComStageFall.h"
#include "ComGoal.h"

void ComMapManager::Init()
{
    MakeMap(ComDataManager::GetInstance().m_stageNum.GetValue());
}

//================================================================================================
//================================================================================================

void ComMapManager::Uninit()
{
    for (auto &itr : m_listMapDate)
    {
        for (auto &itr2 : m_listMapDate[itr.first])
        {
            for (auto &itr3 : m_listMapDate[itr.first][itr2.first])
            {
                Destroy(&itr3.second);
            }
        }
    }
}

//================================================================================================
//================================================================================================

void ComMapManager::MakeMap(int num)
{
    //生成に必要な情報をそれぞれを取得する
    auto mapdata = ComDataManager::GetInstance().m_mapsData[num - 1];
    auto mapsize = ComDataManager::GetInstance().m_mapSize.GetValue();

    //座標を連動させる
    auto nowheight_startpos = 0.0f;

    auto depthmax = static_cast<int>(mapdata.size());

    //全長を計算し半分にしてから反転
    float nowdepth_startpos = static_cast<float>(depthmax * mapsize);
    nowdepth_startpos = -1.0f * nowdepth_startpos / 2;

    //手前から奥
    for (auto &itr : mapdata)
    {

        //左から右
        for (auto &itr2 : itr.second)
        {
            auto widthmax = static_cast<int>(itr.second.size());

            //全長を計算し半分にしてから反転
            float nowwidth_startpos = static_cast<float>((widthmax - 1) * mapsize);
            nowwidth_startpos = -1.0f * nowwidth_startpos / 2;

            std::string objname = "stage :  " + std::to_string(itr2.first);
            objname += ": y " + std::to_string(0);
            objname += ": z " + std::to_string(itr.first);

            auto obj = m_gameObject->AddChildObject(objname, E_TYPE_OBJECT::NONE);
            obj->AddComponent<ComShader>();
            obj->m_typeObject = E_TYPE_OBJECT::MODEL3D;

            auto pos_x = nowwidth_startpos + mapsize * itr2.first;
            auto pos_y = 0.0f;
            auto pos_z = nowdepth_startpos + mapsize * itr.first;

            auto transform = obj->m_transform;

            //各マップチップごとの処理
            switch (itr2.second)
            {
            case E_MAPCHIP::FLOOR:
            case E_MAPCHIP::FLOOR_FALL:
                obj->AddComponent<ComBox>();
                obj->AddComponent<ComBoxCollider3D>()->m_isTrigger.SetValue(true);

                transform->m_size.SetValue(mapsize, mapsize, mapsize);
                transform->m_worldPosition.SetValue(pos_x, pos_y, pos_z);
                if (itr2.second == E_MAPCHIP::FLOOR_FALL)
                {
                    float max = static_cast<float>(depthmax);
                    float nowpos = static_cast<float>(itr.first);
                    float rate = nowpos / max;
                    rate = 1.0f - rate;

                    float hitpoint = 256.0f * rate;

                    //ステージ数によって難易度の調整 小さいほど体力増
                    if (num < 4)
                    {
                        hitpoint += 100.0f - num * 30.0f;
                    }
                    else
                    {
                        hitpoint -= num * 10;
                    }

                    if (hitpoint > 256.0f)
                    {
                        hitpoint = 256.0f;
                    }
                    else if (hitpoint < 40.0f)
                    {
                        hitpoint = 40.0f;
                    }

                    obj->AddComponent<ComStageFall>()->m_count.SetValue(hitpoint);

                    //落ちるオブジェクト

                    obj->m_transform->m_color.SetValue(2.0f, 200.0f, 200.0f, 1.0f);
                }
                break;
            case E_MAPCHIP::GOAL:
                obj->AddComponent<ComBox>();
                obj->AddComponent<ComBoxCollider3D>()->m_isTrigger.SetValue(true);
                obj->AddComponent<ComGoal>();

                transform->m_size.SetValue(mapsize, mapsize, mapsize);
                transform->m_worldPosition.SetValue(pos_x, pos_y, pos_z);
                obj->m_transform->m_color.SetValue(256.0f, 0.0f, 200.0f, 1.0f);

                break;

            case E_MAPCHIP::START:
                obj->AddComponent<ComBox>();
                obj->AddComponent<ComBoxCollider3D>()->m_isTrigger.SetValue(true);

                transform->m_size.SetValue(mapsize, mapsize, mapsize);
                transform->m_worldPosition.SetValue(pos_x, pos_y, pos_z);
                obj->m_transform->m_color.SetValue(200.0f, 0.0f, 200.0f, 1.0f);
                GameObject::Find("player")->m_transform->m_worldPosition.SetValue(pos_x, 30.0f, pos_z);

                break;
            default:
                break;
            }
        }
    }
}
