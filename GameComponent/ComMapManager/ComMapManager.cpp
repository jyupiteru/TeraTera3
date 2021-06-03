#include "ComMapManager.h"

ComMapManager *ComMapManager::m_instance = nullptr;

void ComMapManager::Init()
{
    m_instance = this;

    m_listMapMaxNum[0] = {10, 10};

    for (int i = 0; i < 10; i++)
    {
        for (int k = 0; k < 10; k++)
        {
            m_listMapData[0][i][k] = E_MAPCHIP::FLOOR;
        }
    }

    m_listMapData[0][4][4] = E_MAPCHIP::ENEMY_GOAL;
    m_listMapData[0][4][5] = E_MAPCHIP::ENEMY_GOAL;
    m_listMapData[0][5][4] = E_MAPCHIP::ENEMY_GOAL;
    m_listMapData[0][5][5] = E_MAPCHIP::ENEMY_GOAL;
}

//================================================================================================
//================================================================================================

void ComMapManager::Uninit()
{
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComMapManager::Ready()
{
}

//================================================================================================
//================================================================================================

void ComMapManager::Update()
{
}

//================================================================================================
//================================================================================================

void ComMapManager::Draw()
{
}

//================================================================================================
//================================================================================================

ComMapManager &ComMapManager::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void ComMapManager::CreateMap(int _stagenum)
{
    float mapsize = 10.0f;

    //全長を計算し半分にしてから反転して奥のスタート座標を計算
    auto depth_startpos = static_cast<float>(m_listMapMaxNum[_stagenum].first * mapsize);
    depth_startpos = -1.0f * depth_startpos / 2;

    //全長を計算し半分にしてから反転して横スタート座標を計算
    auto width_startpos = static_cast<float>((m_listMapMaxNum[_stagenum].second) * mapsize);
    width_startpos = -1.0f * width_startpos / 2;

    //手前から奥
    for (int nowdepth = 0; nowdepth < m_listMapMaxNum[_stagenum].first; nowdepth++)
    {
        //左から右
        for (int nowwidth = 0; nowwidth < m_listMapMaxNum[_stagenum].second; nowwidth++)
        {
            //表示する際の名前を作成
            std::string objname = "stage : z " + std::to_string(nowdepth);
            objname += +": x " + std::to_string(nowwidth);

            auto pos_x = width_startpos + mapsize * nowwidth;
            auto pos_z = depth_startpos + mapsize * nowdepth;
            GameObject *obj = nullptr;
            switch (m_listMapData[_stagenum][nowdepth][nowwidth])
            {
            case E_MAPCHIP::FLOOR:
                //オブジェクト作成
                obj = MakeMapObj(objname);

                //座標の計算
                obj->m_transform->m_worldPosition.SetValue(pos_x, 0, pos_z);
                obj->m_transform->m_size.SetValue(mapsize, mapsize, mapsize);

                if (auto color = nowdepth + nowwidth; color % 2 == 0)
                {
                    obj->m_transform->m_color.SetValue(200.0f, 200.0f, 200.0f, 1.0f);
                }

                break;

            case E_MAPCHIP::ENEMY_GOAL:

                //オブジェクト作成
                obj = MakeMapObj(objname);

                //座標の計算
                obj->m_transform->m_worldPosition.SetValue(pos_x, 0, pos_z);
                obj->m_transform->m_size.SetValue(mapsize, mapsize, mapsize);

                obj->m_transform->m_color.SetValue(256.0f, 0.0f, 0.0f, 1.0f);

                break;
            case E_MAPCHIP::NONE:
                break;
            }
        }
    }
}

//================================================================================================
//================================================================================================

GameObject *ComMapManager::MakeMapObj(std::string_view _objname)
{
    //オブジェクト作成
    GameObject *obj = m_gameObject->AddChildObject(_objname.data(), E_TYPE_OBJECT::NONE);
    obj->m_typeObject = E_TYPE_OBJECT::MODEL3D;
    obj->AddComponent<ComShader>();
    obj->AddComponent<ComBox>();
    obj->AddComponent<ComBoxCollider3D>()->m_isTrigger.SetValue(true);
    obj->GetComponent<ComBoxCollider3D>()->m_isFirstJustSize = true;

    return obj;
}