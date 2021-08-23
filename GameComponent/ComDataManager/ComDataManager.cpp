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
	m_stageNum.SetValue(0);
	m_mapSize.SetValue(10.0f);
	LoadMap();
	//SetMap();

	m_instance = this;
	this->m_gameObject->DontDestroyOnLoad();
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

void ComDataManager::LoadMap()
{

	std::string mapname;
	int mapnum = 0;

	while (1)
	{
		//マップの名前を設定
		mapname = "Assets/Maps/";
		mapname += "map" + std::to_string(mapnum);
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

		int depth = 0;
		std::string str;

		//1行取得して分類格納
		while (std::getline(mapfile, str))
		{
			std::string tmp = "";
			std::istringstream stream(str);

			int width = 0;

			while (std::getline(stream, tmp, ','))
			{
				int objnum = std::atoi(tmp.c_str());
				m_mapsData[mapnum - 1][depth][width] = static_cast<E_MAPCHIP>(objnum);
				width++;
			}
			depth++;
		}

		mapnum++;
	}
}