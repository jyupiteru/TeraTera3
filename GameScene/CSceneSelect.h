#include "../../TeraTera3/Managers/SceneSystem/CScene.h"
#pragma once

class SceneMenu : public CScene
{
public:
	SceneMenu() {};
	void Init() override;
	void Uninit() override;

	void Update() override;
	void Draw() override;
};
