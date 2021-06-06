#include "ComShotManager.h"
#include "../ComMapManager/ComMapManager.h"
#include <random>

ComShotManager *ComShotManager::m_instance = nullptr;

void ComShotManager::Init()
{
    m_instance = this;
    m_intervalTime.SetValue(4.0f);
    m_shotMax.SetValue(10);

    m_shotSpeed.SetValue(5, 10);

    m_shotRandNum.SetValue(10);
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
    //同時生成可能数の3倍生成する
    for (int i = 0; i < m_shotMax.GetValue() * 3; i++)
    {
        //オブジェクトの生成
        std::string name = "Shot No" + std::to_string(i);
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
    m_playerobj = GameObject::Find("player");
}

//================================================================================================
//================================================================================================

void ComShotManager::Update()
{
    m_timeCount += CTimer::GetInstance().m_deltaTime.GetValue();

    //弾の生成周期の待機時間を過ぎたか？
    if (m_timeCount > m_intervalTime.GetValue())
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
    if (m_listWaitCreate.size() > 0)
    {
        ComShot *comshot = m_listWaitCreate[0];
        m_listWaitCreate.erase(m_listWaitCreate.begin());
        comshot->m_gameObject->m_activeFlag.SetValue(true);

         DirectX::XMFLOAT3 firstpos;
            //乱数を生成
        std::mt19937 mt{std::random_device{}()};
        {
            firstpos.x = static_cast<float>(ComMapManager::GetInstance().m_mapMax.GetValue().first);
            firstpos.z = static_cast<float>(ComMapManager::GetInstance().m_mapMax.GetValue().second);
            firstpos.y = ComMapManager::GetInstance().m_MaphalfSize.GetValue();

            std::uniform_int_distribution<int> rand(0, 3);

            //ビット操作をする
            std::bitset<4> bit;
            bit = rand(mt);

            if (bit.test(0))
            {
                //右端に生成
                firstpos.x *= firstpos.y * 2;
            }
            else
            {
                //左端に生成
                firstpos.x *= -firstpos.y * 2;
            }

            if (bit.test(1))
            {
                //奥に生成
                firstpos.z *= firstpos.y * 2;
            }
            else
            {
                //手前に生成
                firstpos.z *= -firstpos.y * 2;
            }

            //座標の格納 yは少し浮かせる(衝突判定回数軽減のため)
            comshot->m_gameObject->m_transform->m_worldPosition.SetValue(firstpos.x, firstpos.y * 1.2f, firstpos.z);
        }

        {
            float size= ComMapManager::GetInstance().m_MaphalfSize.GetValue();
            comshot->m_gameObject->m_transform->m_worldPosition.SetValue(size, size, size);
        }

        {
            auto [playerpos_x, playerpos_y, playerpos_z] = m_playerobj->m_transform->m_worldPosition.GetValue();

            std::uniform_int_distribution<int> rand2(0, static_cast<int>(m_shotRandNum.GetValue() * 10));

            float vector_ = rand2(mt);
            vector_ /= 10.0f;

            //発射方向の計算
            std::pair<float, float> vector;
            vector.first = playerpos_x - firstpos.x + vector_ / 2;
            vector.second = playerpos_y - firstpos.z + vector_ / 2;

            //正規化
            float normal = std::sqrt(vector.first * vector.first + vector.second * vector.second);
            vector.first /= normal;
            vector.second /= normal;

            //速さをかける
            std::uniform_int_distribution<int> rand3(static_cast<int>(m_shotSpeed.GetValue().first * 10), static_cast<int>(m_shotSpeed.GetValue().second * 10));
            float speed = rand3(mt);
            speed /= 10.0f;
            vector.first *= speed;
            vector.second += speed;

            comshot->m_gameObject->m_transform->m_vector.SetValue(vector.first, 0, vector.second);
        }
    }
}