

#include "../GameComponents.h"

void ComMapMaker::Init()
{
    m_mapSize.SetValue(10.0f);
    SetMap();
}

void ComMapMaker::Uninit()
{
}

void ComMapMaker::Ready()
{
}

void ComMapMaker::Update()
{
}

void ComMapMaker::Draw()
{
}

void ComMapMaker::MakeMap(int num)
{
    num--;
    //ComMapManagerはこのオブジェクトに追加されているか
    if (auto mapmanager = m_gameObject->GetComponent<ComMapManager>(); mapmanager != nullptr)
    {
        auto mapdata = &m_gameObject->GetComponent<ComMapManager>()->m_listMapDate;

        auto mapsize = m_mapSize.GetValue();

        auto heightmax = static_cast<int>(m_mapsData[num].size());

        auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue();

        //座標を連動させる
        auto nowheight_startpos = pos_y;

        //下から上のループ
        for (auto &itr : m_mapsData[num])
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
}

void ComMapMaker::SetMap()
{
    //enum classをこの関数内で簡単に使用するために
    using enum E_MAPCHIP;

    //左下を0,0とし通常面を高さ0とする
    int stagemax = 1;

    int height = 0;
    int width = 0;
    int depth = 0;

    //stagenum数分のステージを読み込む
    for (int stagenum = 0; stagenum < stagemax; ++stagenum)
    {
        auto stage = &m_mapsData[stagenum];

        //いくつ目のステージか
        switch (stagenum)
        {
        case 0: //1個目のステージ情報

            //一括設定
            height = 1;
            width = 5;
            depth = 30;

            //高さ
            for (auto nowheight = 0; nowheight < height; ++nowheight)
            {
                //奥行
                for (auto nowdepth = 0; nowdepth < depth; ++nowdepth)
                {
                    switch (nowdepth)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 27:
                    case 28:
                    case 29:
                        //横
                        for (auto nowwidth = 0; nowwidth < width; ++nowwidth)
                        {
                            //手前から1個目かつ真ん中にスタートを設置
                            if (nowdepth == 1 && (width / 2) - 1 < nowwidth && nowwidth < (width / 2) + 1)
                            {
                                (*stage)[nowheight][nowdepth][nowwidth] = E_MAPCHIP::START;
                            }
                            else if (nowdepth == (depth - 1) &&
                                     (width / 2) - 1 < nowwidth && nowwidth < (width / 2) + 1)
                            {
                                (*stage)[nowheight][nowdepth][nowwidth] = E_MAPCHIP::GOAL;
                            }
                            else
                            {
                                (*stage)[nowheight][nowdepth][nowwidth] = E_MAPCHIP::FLOOR;
                            }
                        }
                        break;

                    default:
                        for (auto nowwidth = 0; nowwidth < width; ++nowwidth)
                        {

                            switch (auto tmp = nowwidth * nowdepth; tmp % width)
                            {
                            case 1:
                            case 2:
                            case 5:
                                (*stage)[nowheight][nowdepth][nowwidth] = E_MAPCHIP::FLOOR;
                                break;
                            default:

                                (*stage)[nowheight][nowdepth][nowwidth] = E_MAPCHIP::FLOOR_FALL;

                                break;
                            }
                        }
                        break;
                    }
                }
            }

            break;
        case 1:
            break;
        }
    }
}