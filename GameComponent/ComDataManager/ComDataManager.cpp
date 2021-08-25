/**
 * @file ComDataManager.cpp
 * @author jupiter ()
 * @brief ComDataManagerの実装が記載されたcpp
 */

#include "ComDataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

ComDataManager *ComDataManager::m_instance = nullptr;

void ComDataManager::Init()
{
	m_instance = this;
	this->m_gameObject->DontDestroyOnLoad();

	m_stageNum.SetValue(0);
	m_mapSize.SetValue(10.0f);
	LoadStage();
	//SetMap();

}

//================================================================================================
//================================================================================================

void ComDataManager::Uninit()
{
	for (auto itr : m_instance->m_stagesData)
	{
		delete itr.second;
	}

	m_instance->m_stagesData.clear();

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

void ComDataManager::LoadStage()
{

	std::string mapname;
	int mapnum = 0;

	while (1)
	{
		//マップの名前を設定
		mapname = "Assets/Maps/";
		mapname += "stage" + std::to_string(mapnum);
		mapname += ".csv";

		std::ifstream mapfile(mapname);

		//読み込みは失敗か？
		if (!mapfile)
		{
			std::string sentence = "Load";
			sentence += mapname;
			sentence += " is Failed";
			CDebugLog::GetInstance().Draw(sentence);
			CDebugLog::GetInstance().Draw("LoadMap is End");
			break;
		}

		m_instance->m_stagesData[mapnum] = new tagLoadStageData();

		int depth = 0;
		std::string str;

		{ //1行目ステージ情報の取得
			std::getline(mapfile, str);

			std::string tmp = "";
			std::istringstream stream(str);

			//最小値
			std::getline(stream, tmp, ',');
			m_stagesData[mapnum]->m_hitPointMin = static_cast<float>(std::atoi(tmp.c_str()));

			//最大値
			std::getline(stream, tmp, ',');
			m_stagesData[mapnum]->m_hitPointMax = static_cast<float>(std::atoi(tmp.c_str()));

			//減少量
			std::getline(stream, tmp, ',');
			m_stagesData[mapnum]->m_decreaseValue = static_cast<float>(std::atoi(tmp.c_str()));

			//大きさ
			std::getline(stream, tmp, ',');
			m_stagesData[mapnum]->m_stageChipSize = static_cast<float>(std::atoi(tmp.c_str()));
		}

		//1行取得して分類格納
		while (std::getline(mapfile, str))
		{
			std::string tmp = "";
			std::istringstream stream(str);

			int width = 0;

			while (std::getline(stream, tmp, ','))
			{
				int objnum = std::atoi(tmp.c_str());
				m_stagesData[mapnum]->m_stageData[depth][width] = static_cast<E_MAPCHIP>(objnum);
				width++;
			}
			depth++;
		}

		mapnum++;
	}
}