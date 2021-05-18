/**
 * @file CTimer.cpp
 * @author jupiter ()
 * @brief CTimerクラスの実装部分を記述してます
 */
#include "CTimer.h"

double CTimer::m_deltaTime;

std::chrono::high_resolution_clock::time_point CTimer::m_lastTime;

void CTimer::Update(void)
{
    auto nowtime = std::chrono::high_resolution_clock::now();
    m_deltaTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(nowtime - m_lastTime).count());
    m_deltaTime /= 1000;
    m_lastTime = nowtime;
}

//================================================================================================
//================================================================================================

float CTimer::GetProgressTime(void)
{
    auto nowtime = std::chrono::high_resolution_clock::now();
    float progresstime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(nowtime - m_lastTime).count());
    progresstime /= 1000;
    return progresstime;
}