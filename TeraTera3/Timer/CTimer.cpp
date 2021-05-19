/**
 * @file CTimer.cpp
 * @author jupiter ()
 * @brief CTimerクラスの実装部分を記述してます
 */
#include "CTimer.h"

CTimer *CTimer::m_instance = nullptr;

CTimer::CTimer()
{
    m_lastTime = std::chrono::high_resolution_clock::now();
}

//================================================================================================
//================================================================================================

CTimer::~CTimer()
{
    //newしたわけではないのでnullptrにするだけでいい
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void CTimer::Update(void)
{
    //現在の経過時間を取得
    auto nowtime = std::chrono::high_resolution_clock::now();

    //前フレームからの経過病を計算しミリ秒へ変換
    double deltaTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(nowtime - m_lastTime).count());
    m_deltaTime = deltaTime / 1000;
    m_lastTime = nowtime;
}

//================================================================================================
//================================================================================================

void CTimer::Create()
{
    //newされているか確認
    if (m_instance == nullptr)
    {
        m_instance = new CTimer();
    }
    else
    {
        //TODO 重複生成されようとしていることを通知する
    }
}

//================================================================================================
//================================================================================================

void CTimer::Delete(bool flag)
{
    //フラグは立っているか
    if (flag)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

//================================================================================================
//================================================================================================

float CTimer::GetProgressTime(void)
{
    //現在の経過時間を取得し前フレームからの経過時間を計算
    auto nowtime = std::chrono::high_resolution_clock::now();
    float progresstime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(nowtime - m_lastTime).count());
    progresstime /= 1000;
    return progresstime;
}

//================================================================================================
//================================================================================================

CTimer &CTimer::GetInstance()
{
    if (m_instance == nullptr)
    {
        Create();
    }
    return *m_instance;
}