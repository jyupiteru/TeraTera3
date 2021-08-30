﻿/**
 * @file ComStageManager.cpp
 * @author jupiter ()
 * @brief ComStageManagerの実装が記述されたcpp
 */

#include "ComStageManager.h"
#include "../ComDataManager/ComDataManager.h"
#include "ComStageFall.h"
#include "ComGoal.h"

void ComStageManager::Init()
{
    MakeStage(ComDataManager::GetInstance().m_stageNum.GetValue());
}

//================================================================================================
//================================================================================================

void ComStageManager::Uninit()
{
    for (auto &itr : m_listStageDate)
    {
        for (auto &itr2 : itr.second)
        {
            for (auto &itr3 : itr2.second)
            {
                Destroy(&itr3.second);
            }
        }
    }
}

//================================================================================================
//================================================================================================

void ComStageManager::MakeStage(int num)
{
    //生成に必要な情報をそれぞれを取得する
    auto stagedata = ComDataManager::GetInstance().m_stagesData[num]->m_stageData;
    float mapsize = ComDataManager::GetInstance().m_stagesData[num]->m_stageChipSize;
    float minstrength = ComDataManager::GetInstance().m_stagesData[num]->m_hitPointMin;
    float maxstrength = ComDataManager::GetInstance().m_stagesData[num]->m_hitPointMax;
    float decreasevalue = ComDataManager::GetInstance().m_stagesData[num]->m_decreaseValue;

    float strengthdiff = maxstrength - minstrength;

    //座標を連動させる
    float nowheight_startpos = 0.0f;

    int depthmax = static_cast<int>(stagedata.size());

    //全長を計算し半分にしてから反転
    float nowdepth_startpos = static_cast<float>(depthmax * mapsize);
    nowdepth_startpos = -1.0f * nowdepth_startpos / 2;

    //手前から奥
    for (auto &itr : stagedata)
    {

        //左から右
        for (auto &itr2 : itr.second)
        {
            int widthmax = static_cast<int>(itr.second.size());

            //全長を計算し半分にしてから反転
            float nowwidth_startpos = static_cast<float>((widthmax - 1) * mapsize);
            nowwidth_startpos = -1.0f * nowwidth_startpos / 2;

            std::string objname = "stage :  " + std::to_string(itr2.first);
            objname += ": y " + std::to_string(0);
            objname += ": z " + std::to_string(itr.first);

            GameObject *obj = m_gameObject->AddChildObject(objname, E_TYPE_OBJECT::NONE);
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

                    //(最大値-最小値)*場所による補正 + 最小値
                    float strength = strengthdiff * rate;
                    strength += minstrength;

                    //各種情報の受け渡し
                    ComStageFall *comfall = obj->AddComponent<ComStageFall>();
                    comfall->m_count.SetValue(strength);
                    comfall->m_decreaseSpeed.SetValue(decreasevalue);

                    //落ちるオブジェクト

                    obj->m_transform->m_color.SetValue(256.0f, strength, strength, 1.0f);
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
