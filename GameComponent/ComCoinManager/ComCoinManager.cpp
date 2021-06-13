/**
 * @file ComCoinManager.cpp
 * @author jupiter ()
 * @brief ComCoinManagerクラスの実装をしているcpp
 */

#include <random>
#include "ComCoinManager.h"
#include "../ComCoin/ComCoin.h"
#include "../ComTimer/ComTimer.h"
#include "../ComMapManager/ComMapManager.h"

ComCoinManager *ComCoinManager::m_instance = nullptr;

void ComCoinManager::Init()
{
    m_instance = this;
}

//================================================================================================
//================================================================================================

void ComCoinManager::Uninit()
{
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComCoinManager::Ready()
{
    //40個生成 足りなくなったら追加する
    for (int i = 0; i < 40; i++)
    {
        CreateCoinObject();
    }
}

//================================================================================================
//================================================================================================

void ComCoinManager::Update()
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

        CreateCoin();
    }
}

//================================================================================================
//================================================================================================

void ComCoinManager::SetWaitList(ComCoin *_coin, bool _flag)
{
    _coin->m_gameObject->m_activeFlag.SetValue(false);
    m_listWaitCreate.push_back(_coin);
    m_coinMax.AddValue(-1);
}

//================================================================================================
//================================================================================================

void ComCoinManager::CreateCoin()
{
    //全体(8割計算)のうちどれくらい進んでいるか
    float timerate = ComTimer::GetInstance().m_maxTimeCount.GetValue() * 0.8f;
    timerate = ComTimer::GetInstance().m_nowCount.GetValue() / timerate;

    //全体の8割過ぎたか？
    if (timerate > 1.0f)
    {
        timerate = 1.0f;
    }

    //生成待ちのコインは存在するか？
    if (m_listWaitCreate.size() <= 0)
    {
        CreateCoinObject();
    }

    std::random_device rd;

    std::mt19937 mt(rd());

    ComCoin *comcoin = m_listWaitCreate[0];
    m_listWaitCreate.erase(m_listWaitCreate.begin());
    comcoin->m_gameObject->m_activeFlag.SetValue(true);
    m_listActiveCoin.push_back(comcoin);

    DirectX::XMFLOAT3 firstpos;

    { //コイン生成場所

        std::pair<int, int> mapmax = ComMapManager::GetInstance().m_mapMax.GetValue();
        firstpos.y = ComMapManager::GetInstance().m_MaphalfSize.GetValue();

        std::uniform_int_distribution<int> rand_x(0, mapmax.first);
        std::uniform_int_distribution<int> rand_z(0, mapmax.second);

        //座標の決定
        firstpos.x = static_cast<float>(rand_x(mt));
        firstpos.z = static_cast<float>(rand_z(mt));

        //マップ上の座標に修正
        firstpos.x /= 2;
        firstpos.z /= 2;

        firstpos.x *= firstpos.y;
        firstpos.z *= firstpos.y;

        comcoin->m_gameObject->m_transform->m_worldPosition.SetValue(firstpos.x, firstpos.y * 1.2f, firstpos.z);
    }
    { //コインの点数
        //comcoin->m_hitScore.SetValue();
    }
    comcoin->m_gameObject->m_transform->m_angle.SetValue(0, 0, 0);
}

//================================================================================================
//================================================================================================

ComCoinManager &ComCoinManager::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void ComCoinManager::CreateCoinObject()
{
    auto [size_x, size_y, size_z] = m_coinSize.GetValue();

    //オブジェクトの生成
    std::string name = "Coin No" + std::to_string(m_listCoinList.size());
    GameObject *coin = m_gameObject->AddChildObject(name, E_TYPE_OBJECT::MODEL3D);
    coin->m_transform->m_size.SetValue(size_x, size_y, size_z);
    coin->m_transform->m_color.SetValue(255, 241, 0, 1.0f);

    //各コンポーネントの設定
    coin->GetComponent<Com3DModelAssimp>()->LoadModelData("Coin/Coin.fbx", "Coin/");

    coin->m_activeFlag.SetValue(false);
    ComCoin *comcoin = coin->AddComponent<ComCoin>();

    //リストへの格納
    m_listWaitCreate.emplace_back(comcoin);
    m_listCoinList.emplace_back(comcoin);

    //衝突判定関係の設定
    ComBoxCollider3D *collider = coin->AddComponent<ComBoxCollider3D>();
    collider->m_isTrigger.SetValue(true);
    collider->m_isFirstJustSize = true;

#ifdef _DEBUG
    collider->m_draw = true;
#endif // _DEBUG
}