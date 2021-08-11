#include "../../TeraTera3/Managers/SceneSystem/CScene.h"
#pragma once

class CSceneSelect : public CScene
{
public:
	CSceneSelect() {};
	void Init() override;
	void Uninit() override;

	void Update() override;
	void Draw() override;
};
