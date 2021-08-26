#include "../../TeraTera3/Managers/SceneSystem/CScene.h"
#pragma once

class CSceneMenu : public CScene
{
public:
	CSceneMenu() {};
	void Init() override;
	void Uninit() override;

	void Update() override;
	void Draw() override;
};
