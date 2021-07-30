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
#include "../ComDataManager/ComDataManager.h"

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
    for (int i = 0; i < m_coinMax.GetValue(); i++)
    {
        CreateCoinObject();
    }

    auto textobj = m_gameObject->AddChildObject("CoinText", E_TYPE_OBJECT::SYSTEM);

    textobj->m_transform->m_worldPosition.SetValue(-SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 60.0f, 0.0f);
    textobj->m_transform->m_size.SetValue(240.0f, 120.0f, 1.0f);
    textobj->m_transform->m_color.SetValue(0.0f, 0.0f, 0.0f, 1.0f);
    m_comScoreText = textobj->AddComponent<Com2DText>();

    m_comScoreText->m_flagSetRate.SetValue(true);
    m_comScoreText->m_text = "CoinTotal : " + std::to_string(m_coinGetCount);
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

    //弾の生成周期の待機時間を過ぎたか？&& 上限以内か？
    if (m_timeCount > time && m_coinMax.GetValue() > m_listActiveCoin.size())
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

    //リストから探しては破棄する
    for (auto itr = m_listActiveCoin.begin(); itr != m_listActiveCoin.end();)
    {
        if ((*itr) == _coin)
        {
            itr = m_listActiveCoin.erase(itr);
            break;
        }
        else
        {
            itr++;
        }
    }

    if (_flag)
    { //プレイヤーに当たったのでスコアを加算
        m_coinGetCount++;
        ComDataManager::GetInstance().m_shotScore.AddValue(_coin->m_hitScore.GetValue());

        m_comScoreText->m_text = "CoinTotal : " + std::to_string(m_coinGetCount);
    }
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

        std::uniform_int_distribution<int> rand_x(0, mapmax.first - 1);
        std::uniform_int_distribution<int> rand_z(0, mapmax.second - 1);

        //座標の決定
        firstpos.x = static_cast<float>(rand_x(mt));
        firstpos.z = static_cast<float>(rand_z(mt));

        firstpos.x -= mapmax.first / 2;
        firstpos.z -= mapmax.second / 2;

        firstpos.x *= firstpos.y;
        firstpos.z *= firstpos.y;

        comcoin->m_gameObject->m_transform->m_worldPosition.SetValue(firstpos.x, firstpos.y * 1.2f, firstpos.z);
    }
    { //コインの点数
        int score = +m_coinScore.GetValue().second - m_coinScore.GetValue().first;

        score = static_cast<int>(timerate * score);
        score += m_coinScore.GetValue().first;

        comcoin->m_hitScore.SetValue(score);
    }
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
    Com3DModelAssimp* commodel = coin->GetComponent<Com3DModelAssimp>();
    commodel->LoadModelData("Coin/Coin.fbx", "Coin/");
    commodel->m_flagDrawShadow = true;

    coin->m_activeFlag.SetValue(false);
    ComCoin *comcoin = coin->AddComponent<ComCoin>();

    //リストへの格納
    m_listWaitCreate.emplace_back(comcoin);
    m_listCoinList.emplace_back(comcoin);

    //衝突判定関係の設定
    ComBoxCollider3D *collider = coin->AddComponent<ComBoxCollider3D>();
    collider->m_isTrigger.SetValue(true);
    collider->m_isFirstJustSize = true;

    //デバッグ時なら衝突範囲を描画する
#ifdef _DEBUG
    collider->m_draw = true;
#endif // _DEBUG
}