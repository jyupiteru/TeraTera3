#include "ComMapManager.h"
#include "../ComShotManager/ComShotManager.h"

ComMapManager *ComMapManager::m_instance = nullptr;

void ComMapManager::Init()
{
    m_instance = this;

    //手前から奥
    for (int i = 0; i < 10; i++)
    {
        //左から右
        for (int k = 0; k < 10; k++)
        {
            m_listMapData[0][i][k] = E_MAPCHIP::FLOOR;
        }
    }


    m_mapMax.SetValue(10, 10);
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
    float mapsize = m_MaphalfSize.GetValue();

    //全長を計算し半分にしてから反転して奥のスタート座標を計算
    auto depth_startpos = static_cast<float>(m_mapMax.GetValue().second * mapsize);
    depth_startpos = -1.0f * depth_startpos / 2;

    //全長を計算し半分にしてから反転して横スタート座標を計算
    auto width_startpos = static_cast<float>(m_mapMax.GetValue().first * mapsize);
    width_startpos = -1.0f * width_startpos / 2;

    //手前から奥
    for (int nowdepth = 0; nowdepth < m_mapMax.GetValue().second; nowdepth++)
    {
        //左から右
        for (int nowwidth = 0; nowwidth < m_mapMax.GetValue().first; nowwidth++)
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

            case E_MAPCHIP::NONE:
            default:
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