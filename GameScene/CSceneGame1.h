
#include "../../TeraTera3/Managers/SceneSystem/CScene.h"

#pragma once

class CSceneGame1 : public CScene
{
public:
	CSceneGame1(){};
	~CSceneGame1(){};
	void Init() override;
	void Uninit() override;

	void Update() override;
	void Draw() override;
};
