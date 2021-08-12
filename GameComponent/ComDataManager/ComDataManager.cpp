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
	m_stageNum.SetValue(1);

	m_mapSize.SetValue(10.0f);
	SetMap();

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

void ComDataManager::SetMap()
{
	//enum classをこの関数内で簡単に使用するために
	using enum E_MAPCHIP;

	//左下を0,0とし通常面を高さ0とする
	int stagemax = 1;

	int width = 0;
	int depth = 0;

	//stagenum数分のステージを読み込む
	for (int stagenum = 0; stagenum < stagemax; ++stagenum)
	{
		auto stage = &m_mapsData[stagenum];

		//いくつ目のステージか
		switch (stagenum)
		{
		case 0: //1個目のステージ情報

			//一括設定
			width = 5;
			depth = 30;

			//奥行
			for (auto nowdepth = 0; nowdepth < depth; ++nowdepth)
			{
				switch (nowdepth)
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 27:
				case 28:
				case 29:
					//横
					for (auto nowwidth = 0; nowwidth < width; ++nowwidth)
					{
						//手前から1個目かつ真ん中にスタートを設置
						if (nowdepth == 1 && (width / 2) - 1 < nowwidth && nowwidth < (width / 2) + 1)
						{
							(*stage)[nowdepth][nowwidth] = E_MAPCHIP::START;
						}
						else if (nowdepth == (depth - 1) &&
								 (width / 2) - 1 < nowwidth && nowwidth < (width / 2) + 1)
						{
							(*stage)[nowdepth][nowwidth] = E_MAPCHIP::GOAL;
						}
						else
						{
							(*stage)[nowdepth][nowwidth] = E_MAPCHIP::FLOOR;
						}
					}
					break;

				default:
					for (auto nowwidth = 0; nowwidth < width; ++nowwidth)
					{

						switch (auto tmp = nowwidth * nowdepth; tmp % width)
						{
						case 1:
						case 2:
						case 5:
							(*stage)[nowdepth][nowwidth] = E_MAPCHIP::FLOOR;
							break;
						default:

							(*stage)[nowdepth][nowwidth] = E_MAPCHIP::FLOOR_FALL;

							break;
						}
					}
					break;
				}
			}

			break;
		case 1:
			break;
		}
	}
}

//================================================================================================
//================================================================================================

void ComDataManager::LoadMap()
{

	std::string mapname;
	int mapnum = 1;

	while (1)
	{
		//マップの名前を設定
		mapname += "Assets/Maps/";
		mapname += "map" + std::to_string(mapnum);
		mapname += ".csv";

		std::ifstream mapfile(mapname);

		int depth = 0;
		int width = 0;

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

		{ //基礎データの読み込み
			int mapcommondata[3];

			//1行目を取得してstrに格納
			std::string str;
			std::getline(mapfile, str);
			std::string tmp = "";
			std::istringstream stream(str);

			int i = 0;
			//","を除いてstrからtmpに格納
			while (std::getline(stream, tmp, ','))
			{
				mapcommondata[i] = std::atoi(tmp.c_str());
				i++;
			}
		}

		mapnum++;
	}
}