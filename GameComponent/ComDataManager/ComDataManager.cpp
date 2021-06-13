/**
 * @file ComDataManager.cpp
 * @author jupiter ()
 * @brief ComDataManagerクラスの実装が記述されたcpp
 */

#include "ComDataManager.h"

ComDataManager *ComDataManager::m_instance = nullptr;

void ComDataManager::Init()
{
    m_instance = this;

    m_maxTime.SetValue(60.0f);
    m_nowCount.SetValue(0.0f);
}

//================================================================================================
//================================================================================================

void ComDataManager::Uninit()
{
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

ComDataManager &ComDataManager::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void ComDataManager::LoadScore()
{
    FILE* file;
    fopen_s(&file, "Data/score.txt", "r");

    if (file)
    {
        int score = 0;
        fscanf_s(file, "%d", &score);

        if (score > 0)
        {
            m_highScore.SetValue(score);
        }

        fclose(file);
    }
}

//================================================================================================
//================================================================================================

void ComDataManager::SaveScore()
{
    FILE* file;
    fopen_s(&file, "Data/score.txt", "w");

    if (file)
    {
        int highscore = m_highScore.GetValue();
        fprintf(file, "%d", highscore);

        fclose(file);
    }
}