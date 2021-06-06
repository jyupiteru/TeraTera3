#include "ComShotManager.h"
#include "../ComMapManager/ComMapManager.h"
#include <random>
#include "../ComTimer/ComTimer.h"

ComShotManager *ComShotManager::m_instance = nullptr;

void ComShotManager::Init()
{
    m_instance = this;
    m_intervalTime.SetValue(4.0f, 2.0f);
    m_shotSize.SetValue(5, 10, 20);

    m_shotSpeed.SetValue(5, 10, 20);

    m_shotRandNum.SetValue(0, 10, 5);
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
    m_playerobj = GameObject::Find("player");
}

//================================================================================================
//================================================================================================

void ComShotManager::Update()
{
    m_timeCount += CTimer::GetInstance().m_deltaTime.GetValue();

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
    std::mt19937 mt{std::random_device{}()};
    {
        firstpos.x = static_cast<float>(ComMapManager::GetInstance().m_mapMax.GetValue().first) / 2;
        firstpos.z = static_cast<float>(ComMapManager::GetInstance().m_mapMax.GetValue().second) / 2;
        firstpos.y = ComMapManager::GetInstance().m_MaphalfSize.GetValue();

        std::uniform_int_distribution<int> rand(0, 3);

        //ビット操作をする
        std::bitset<4> bit;
        bit = rand(mt);

        if (bit.test(0))
        {
            //右端に生成
            firstpos.x *= firstpos.y;
        }
        else
        {
            //左端に生成
            firstpos.x *= -firstpos.y;
        }

        if (bit.test(1))
        {
            //奥に生成
            firstpos.z *= firstpos.y;
        }
        else
        {
            //手前に生成
            firstpos.z *= -firstpos.y;
        }

        //座標の格納 yは少し浮かせる(衝突判定回数軽減のため)
        comshot->m_gameObject->m_transform->m_worldPosition.SetValue(firstpos.x, firstpos.y * 1.2f, firstpos.z);
    }

    {
        //大きさの処理
        auto [size_min, size_firstmax, size_lastmax] = m_shotSize.GetValue();

        float sizemax = size_lastmax - size_firstmax;
        sizemax *= timerate;
        sizemax += size_firstmax;

        std::uniform_int_distribution<int> rand3(static_cast<int>(size_min * 10), static_cast<int>(sizemax * 10));

        float nowsize = rand3(mt);
        nowsize /= 10;

        comshot->m_gameObject->m_transform->m_size.SetValue(nowsize, nowsize, nowsize);
    }

    {
        auto [playerpos_x, playerpos_y, playerpos_z] = m_playerobj->m_transform->m_worldPosition.GetValue();

        auto [rand_min, rand_firstmax, rand_lastmax] = m_shotRandNum.GetValue();

        float nowrand = rand_firstmax + rand_lastmax;
        nowrand *= 2;
        std::uniform_int_distribution<int> rand2(static_cast<int>(rand_min * 10), static_cast<int>(nowrand * 10));

        std::pair<float, float> vector_;
        vector_.first = rand2(mt);
        vector_.first /= 10.0f;
        vector_.first -= nowrand;

        vector_.second = rand2(mt);
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
        float speed = rand3(mt);
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
    shot->m_transform->m_color.SetValue(255, 0, 255, 1.0f);
    shot->m_activeFlag.SetValue(false);
    ComShot *comshot = shot->AddComponent<ComShot>();

    //リストへの格納
    m_listWaitCreate.emplace_back(comshot);
    m_listShotList.emplace_back(comshot);

    //衝突判定関係の設定
    ComBoxCollider3D *collider = shot->AddComponent<ComBoxCollider3D>();
    collider->m_isTrigger.SetValue(true);
    collider->m_draw = true;
    collider->m_isFirstJustSize = true;
}