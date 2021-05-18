
#include "../../Template/SceneSystem/CScene.h"
#pragma once

class CSceneTitle : public CScene
{
public:
	CSceneTitle(){};
	void Init() override;
	void Uninit() override;

	void Update() override;
	void Draw() override;
};
