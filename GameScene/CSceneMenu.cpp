#include "CSceneMenu.h"

void CSceneMenu::Init()
{
	{
		GameObject *menumanager = GameObject::MakeNewObject("MenuManager", E_TYPE_OBJECT::SYSTEM);
		ComMenuManager *comtitle = menumanager->AddComponent<ComMenuManager>();
		//comtitle->m_titleImageObj = GameObject::Find("TitleImage");
	}

	{
		auto selector = GameObject::MakeNewObject("selector", E_TYPE_OBJECT::UI);
		selector->GetComponent<Com2DTexture>()->LoadTexture("selector.png");
		selector->m_transform->m_worldPosition.SetValue(0.0f, -200, 0.0f);
		selector->m_transform->m_size.SetValue(630.0f, 50.0f, 1);
		selector->m_transform->m_color.SetValue(256.0f, 256.0f, 256.0f, 1.0f);
		auto comselect = selector->AddComponent<ComMenuSelectorMove>();
		comselect->m_widthMaxPlusSize = 30.0f;
		comselect->m_plusSizePerSecond = 30.0f;
	}

	{
		GameObject *stagenum = GameObject::MakeNewObject("StageNum", E_TYPE_OBJECT::UI);
		stagenum->m_transform->m_worldPosition.SetValue(0.0f, -100, 0.0f);
		stagenum->m_transform->m_size.SetValue(500.0f, 100.0f, 1);
		stagenum->m_transform->m_color.SetValue(0.0f, 0.0f, 0.0f, 1.0f);

		stagenum->RemoveComponent<Com2DTexture>();
		Com2DText *text = stagenum->AddComponent<Com2DText>();
		text->m_text = "Stage : 1";
		text->m_flagSetRate.SetValue(true);
	}

	{
		GameObject *player = GameObject::MakeNewObject("player", E_TYPE_OBJECT::MODEL3D);
		player->m_transform->m_worldPosition.SetValue(8.0f, 0.0f, -2.0f);
		player->m_transform->m_angle.SetValue(0.0f, 140.0f, 0.0f);
		player->m_transform->m_size.SetValue(8.0f, 8.0f, 8.0f);
		player->GetComponent<Com3DModelAssimp>()->LoadModelData("Player/idle_run.fbx", "Player/");
		player->AddComponent<ComPlayerTitle>();

		Com3DAnimationAssimp *anim = player->AddComponent<Com3DAnimationAssimp>();

		anim->LoadAnimation("idle", "Player/idle_run.fbx");
		anim->SetAnimationName("idle", 0, "idle");
		anim->SetAnimationName("idle", 1, "run");

		anim->LoadAnimation("stretch", "Player/stretch.fbx");
		anim->SetAnimationName("stretch", 0, "neck");
		anim->SetAnimationName("stretch", 1, "arm");

		anim->LoadAnimation("jump", "Player/jump.fbx");
		anim->SetAnimationName("jump", 0, "jumpup");
		anim->SetAnimationName("jump", 1, "falling");
		anim->SetAnimationName("jump", 2, "jumpdown");

		anim->ChangeAnimation("neck");
	}

	{
		auto floor = GameObject::MakeNewObject("floor", E_TYPE_OBJECT::MODEL3D);
		floor->RemoveComponent<Com3DModelAssimp>();
		floor->AddComponent<ComBox>();
		floor->m_transform->m_size.SetValue(40.0f, 1.0f, 40.0f);
		floor->m_transform->m_worldPosition.SetValue(0.0f, -5.0f, 0.0f);
		floor->m_transform->m_color.AddValue(-100.0f, -100.0f, -100.0f, 0.0f);
	}

	{
		auto camera = GameObject::Find("Camera");
		camera->m_transform->m_worldPosition.SetValue(0.0f, 4.0f, 0.0f);
		auto comcamera = camera->GetComponent<ComCamera>();
		comcamera->m_typeFixed = E_TYPE_FIXED::LOOKAT;
		comcamera->m_range.SetValue(10);
		comcamera->m_firstAngle.SetValue(0.0f, 0.0f, 0.0f);
	}
}

void CSceneMenu::Uninit()
{
}

void CSceneMenu::Update()
{
}

void CSceneMenu::Draw()
{
}