/**
 * @file ComShotManager.cpp
 * @author jupiter ()
 * @brief ComShotManagerクラスの実装を記述したcpp
 */

#include <random>
#include "ComShotManager.h"
#include "../ComMapManager/ComMapManager.h"
#include "../ComTimer/ComTimer.h"

ComShotManager *ComShotManager::m_instance = nullptr;

void ComShotManager::Init()
{
    m_instance = this;
    m_intervalTime.SetValue(4.0f, 2.0f);

    m_shotSize.SetValue(1.0f, 0.5f, 0.5f);
    m_shotSizeRate.SetValue(5.0f, 10.0f, 20.0f);

    m_shotSpeed.SetValue(5.0f, 10.0f, 20.0f);

    m_shotRandNum.SetValue(0.0f, 10.0f, 5.0f);
}

//================================================================================================
//================================================================================================

void ComShotManager::Uninit()
{
    m_listShotList.clear();
    m_listWaitCreate.clear();
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComShotManager::Ready()
{
    //とりあえず30個生成 足りなくなったら追加する
    for (int i = 0; i < 30; i++)
    {
        CreateShotObject();
    }
    m_playerobj = GameObject::Find("Player");
}

//================================================================================================
//================================================================================================

void ComShotManager::Update()
{
    m_timeCount += static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    //現在の生成周期の計算
    float maxtime = ComTimer::GetInstance().m_maxTimeCount.GetValue();
    float timerate = ComTimer::GetInstance().m_nowCount.GetValue() / maxtime;
    float time = m_intervalTime.GetValue().first - m_intervalTime.GetValue().second;
    time *= timerate;
    time = m_intervalTime.GetValue().first - time;

    //弾の生成周期の待機時間を過ぎたか？
    if (m_timeCount > time)
    {
        m_timeCount = 0.0f;

        CrateShot();
    }

    UpdateShots();
}

//================================================================================================
//================================================================================================

ComShotManager &ComShotManager::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void ComShotManager::SetShotReadyList(ComShot *_shot)
{
    _shot->m_gameObject->m_activeFlag.SetValue(false);
    m_listWaitCreate.push_back(_shot);
}

//================================================================================================
//================================================================================================

void ComShotManager::CrateShot()
{
    //生成待ちの弾数はあるか？
    if (m_listWaitCreate.size() <= 0)
    {
        CreateShotObject();
    }

    ComShot *comshot = m_listWaitCreate[0];
    m_listWaitCreate.erase(m_listWaitCreate.begin());
    comshot->m_gameObject->m_activeFlag.SetValue(true);
    m_listActiveShot.push_back(comshot);

    //全体(8割計算)のうちどれくらい進んでいるか
    float timerate = ComTimer::GetInstance().m_maxTimeCount.GetValue() * 0.8f;
    timerate = ComTimer::GetInstance().m_nowCount.GetValue() / timerate;

    //全体の8割過ぎたか？
    if (timerate > 1.0f)
    {
        timerate = 1.0f;
    }

    DirectX::XMFLOAT3 firstpos;
    //乱数を生成
    std::random_device rd;

    std::mt19937 mt(rd());

    {
        std::pair<int, int> mapmax = ComMapManager::GetInstance().m_mapMax.GetValue();
        /*mapmax.first /= 2;
        mapmax.second /= 2;*/

        firstpos.y = ComMapManager::GetInstance().m_MaphalfSize.GetValue();

        std::uniform_int_distribution<int> rand(0, 3);
        std::uniform_int_distribution<int> rand_x(-mapmax.first, mapmax.first);
        std::uniform_int_distribution<int> rand_z(-mapmax.second, mapmax.second);

        int randnum = rand(mt);

        switch (randnum)
        {
        case 0: //右
            firstpos.x = static_cast<float>(mapmax.first);
            firstpos.z = static_cast<float>(rand_z(mt));
            break;

        case 1: //左
            firstpos.x = static_cast<float>(-mapmax.first);
            firstpos.z = static_cast<float>(rand_z(mt));
            break;

        case 2: //上
            firstpos.z = static_cast<float>(mapmax.second);
            firstpos.x = static_cast<float>(rand_x(mt));
            break;

        case 3: //下
            firstpos.z = static_cast<float>(-mapmax.second);
            firstpos.x = static_cast<float>(rand_x(mt));
            break;
        }

        std::string drawstring = "rand " + std::to_string(randnum);
        drawstring += " X " + std::to_string(firstpos.x);
        drawstring += " Z : " + std::to_string(firstpos.z);
        CDebugLog::GetInstance().Draw(drawstring);

        firstpos.x *= firstpos.y;
        firstpos.z *= firstpos.y;

        //座標の格納 yは少し浮かせる(衝突判定回数軽減のため)
        comshot->m_gameObject->m_transform->m_worldPosition.SetValue(firstpos.x, firstpos.y * 1.2f, firstpos.z);
    }

    {
        //大きさの処理
        auto [size_x, size_y, size_z] = m_shotSize.GetValue();
        auto [size_min, size_firstmax, size_lastmax] = m_shotSizeRate.GetValue();

        float sizemax = size_lastmax - size_firstmax;
        sizemax *= timerate;
        sizemax += size_firstmax;

        std::uniform_int_distribution<int> rand3(static_cast<int>(size_min * 10), static_cast<int>(sizemax * 10));

        float nowsize = static_cast<float>(rand3(mt));
        nowsize /= 10;

        comshot->m_gameObject->m_transform->m_size.SetValue(nowsize * size_x, nowsize * size_y, nowsize * size_z);
    }

    {
        auto [playerpos_x, playerpos_y, playerpos_z] = m_playerobj->m_transform->m_worldPosition.GetValue();

        auto [rand_min, rand_firstmax, rand_lastmax] = m_shotRandNum.GetValue();

        //ずれの計算
        float nowrand = rand_firstmax + rand_lastmax;
        nowrand *= 2;
        std::uniform_int_distribution<int> rand2(static_cast<int>(rand_min * 10), static_cast<int>(nowrand * 10));

        //x,zのペア
        std::pair<float, float> vector_;
        vector_.first = static_cast<float>(rand2(mt));
        vector_.first /= 10.0f;
        vector_.first -= nowrand;

        vector_.second = static_cast<float>(rand2(mt));
        vector_.second /= 10.0f;
        vector_.second -= nowrand;

        //発射方向の計算
        std::pair<float, float> vector;
        vector.first = playerpos_x - firstpos.x + vector_.first;
        vector.second = playerpos_z - firstpos.z + vector_.second;

        //正規化
        float normal = std::sqrt(vector.first * vector.first + vector.second * vector.second);
        vector.first /= normal;
        vector.second /= normal;

        auto [speed_min, speed_firstmax, speed_lastmax] = m_shotSpeed.GetValue();

        //速さの上限の計算
        float nowspeed = speed_lastmax - speed_firstmax;
        nowspeed *= timerate;
        nowspeed += speed_firstmax;

        //速さをかける
        std::uniform_int_distribution<int> rand3(static_cast<int>(speed_min * 10), static_cast<int>(nowspeed * 10));
        float speed = static_cast<float>(rand3(mt));
        speed /= 10.0f;
        vector.first *= speed;
        vector.second *= speed;
        comshot->m_shotSpeed.SetValue(vector.first, 0, vector.second);
    }
}

//================================================================================================
//================================================================================================

void ComShotManager::CreateShotObject()
{
    //オブジェクトの生成
    std::string name = "Shot No" + std::to_string(m_listShotList.size());
    GameObject *shot = m_gameObject->AddChildObject(name, E_TYPE_OBJECT::MODEL3D);

    //各コンポーネントの設定
    shot->RemoveComponent<Com3DModelAssimp>();
    shot->AddComponent<ComSphere>();
    shot->m_transform->m_color.SetValue(125, 125, 255, 1.0f);
    shot->m_activeFlag.SetValue(false);

    ComShader *comshader = shot->GetComponent<ComShader>();
    comshader->LoadPixelShader("PSAmbient.fx", true);

    //リストへの格納
    ComShot *comshot = shot->AddComponent<ComShot>();
    m_listWaitCreate.emplace_back(comshot);
    m_listShotList.emplace_back(comshot);

    //衝突判定関係の設定
    ComBoxCollider3D *collider = shot->AddComponent<ComBoxCollider3D>();
    collider->m_isTrigger.SetValue(true);
    collider->m_isFirstJustSize = true;

#ifdef _DEBUG
    collider->m_draw = true;
#endif // _DEBUG
}

//================================================================================================
//================================================================================================

void ComShotManager::UpdateShots()
{
    //マップの大きさを取り出し計算
    std::pair<int, int> mapmax = ComMapManager::GetInstance().m_mapMax.GetValue();
    float maphalfsize = ComMapManager::GetInstance().m_MaphalfSize.GetValue();
    std::pair<float, float> mapsize;
    mapsize.first = maphalfsize * 2 * mapmax.first;
    mapsize.second = maphalfsize * 2 * mapmax.second;

    for (auto itr = m_listActiveShot.begin(); itr != m_listActiveShot.end();)
    {
        //座標を取り出し有効範囲内なら更新 外に出たら非アクティブに
        auto [pos_x, pos_y, pos_z] = (*itr)->m_gameObject->m_transform->m_worldPosition.GetValue();
        if (-mapsize.first < pos_x && pos_x < mapsize.first &&
            -mapsize.second < pos_z && pos_z < mapsize.second)
        {
            (*itr)->UpdateShot();
            itr++;
        }
        else
        {
            this->SetShotReadyList(*itr);
            itr = m_listActiveShot.erase(itr);
        }
    }
}