
#include "../../TeraTera3/Managers/SceneSystem/CScene.h"

#pragma once

class CSceneResult : public CScene
{
public:
	CSceneResult(){};
	void Init() override;
	void Uninit() override;

	void Update() override;
	void Draw() override;
};
