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

    auto heightmax = static_cast<int>(mapdata.size());

    auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue();

    //座標を連動させる
    auto nowheight_startpos = pos_y;

    //下から上のループ
    for (auto &itr : mapdata)
    {

        auto depthmax = static_cast<int>(itr.second.size());

        //全長を計算し半分にしてから反転
        auto nowdepth_startpos = static_cast<float>(depthmax * mapsize);
        nowdepth_startpos = -1.0f * nowdepth_startpos / 2;

        //手前から奥
        for (auto &itr2 : itr.second)
        {
            auto widthmax = static_cast<int>(itr2.second.size());

            //全長を計算し半分にしてから反転
            auto nowwidth_startpos = static_cast<float>((widthmax - 1) * mapsize);
            nowwidth_startpos = -1.0f * nowwidth_startpos / 2;

            //左から右
            for (auto &itr3 : itr2.second)
            {
                std::string objname = "stage :  " + std::to_string(itr3.first);
                objname += ": y " + std::to_string(itr.first);
                objname += ": z " + std::to_string(itr2.first);

                auto obj = m_gameObject->AddChildObject(objname, E_TYPE_OBJECT::NONE);
                obj->AddComponent<ComShader>();
                obj->m_typeObject = E_TYPE_OBJECT::MODEL3D;

                auto pos_x = nowwidth_startpos + mapsize * itr3.first;
                auto pos_y = nowheight_startpos + mapsize * itr.first;
                auto pos_z = nowdepth_startpos + mapsize * itr2.first;

                auto transform = obj->m_transform;

                //各マップチップごとの処理
                switch (itr3.second)
                {
                case E_MAPCHIP::FLOOR:
                case E_MAPCHIP::FLOOR_FALL:
                    obj->AddComponent<ComBox>();
                    obj->AddComponent<ComBoxCollider3D>()->m_isTrigger.SetValue(true);

                    transform->m_size.SetValue(mapsize, mapsize, mapsize);
                    transform->m_worldPosition.SetValue(pos_x, pos_y, pos_z);
                    if (itr3.second == E_MAPCHIP::FLOOR_FALL)
                    {
                        //ここで分岐による耐久値の設定
                        if (itr2.first < 20)
                        { //最初はちょっとずつ難しく
                            obj->AddComponent<ComStageFall>()->m_count.SetValue(256.0f - itr2.first * 3);
                        }
                        else
                        { //最後はかなり難しく
                            obj->AddComponent<ComStageFall>()->m_count.SetValue(100);
                        }
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
}
