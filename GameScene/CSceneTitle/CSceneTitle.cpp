#include "CSceneTitle.h"

void CSceneTitle::Init()
{
#ifndef _DEBUG
    //デバッグじゃないときImGui表示の削除
    CImGuiManager::GetInstance()->m_flagSurvival.SetValue(false);
#endif // DEBUG

    { //ステージ（ドーム表示）
        if (GameObject *skydome = GameObject::Find("skydome"); skydome == nullptr)
        {
            skydome = GameObject::MakeNewObject("skydome", E_TYPE_OBJECT::NONE);
            skydome->AddPackage<Package3DModelObject>();
            skydome->GetComponent<Com3DModel>()->LoadModelData("skydome.x.dat");
            skydome->GetComponent<ComShader>()->LoadPixelShader("PSOnlyTex.fx", true);
            skydome->m_transform->m_size.SetValue(10.0f, 10.0f, 10.0f);
            skydome->DontDestroyOnLoad();
            skydome->m_drawLayer.SetValue(-1);
        }
    }

    {
        auto title = GameObject::MakeNewObject("title", E_TYPE_OBJECT::UI);
        title->GetComponent<Com2DTexture>()->LoadTexture("gametitle.png");
        title->m_transform->m_worldPosition.SetValue(0, SCREEN_HEIGHT / 6 * 2, 0);
        title->m_transform->m_size.SetValue(620.0f, 215.0f, 1.0f);
        title->m_transform->m_color.SetValue(256.0f, 256.0f, 256.0f, 1.0f);
    }

    {
        auto selectimage = GameObject::MakeNewObject("selectimage", E_TYPE_OBJECT::UI);
        selectimage->GetComponent<Com2DTexture>()->LoadTexture("pressenter.png");
        selectimage->m_transform->m_worldPosition.SetValue(0.0f, -SCREEN_HEIGHT / 6 * 2, 0.0f);
        selectimage->m_transform->m_size.SetValue(530.0f, 36.0f, 1.0f);
        selectimage->m_transform->m_color.SetValue(256.0f, 256.0f, 256.0f, 1.0f);
    }

    {
        auto selector = GameObject::MakeNewObject("selector", E_TYPE_OBJECT::UI);
        selector->GetComponent<Com2DTexture>()->LoadTexture("selector.png");
        selector->m_transform->m_worldPosition.SetValue(0.0f, -SCREEN_HEIGHT / 6 * 2, 0.0f);
        selector->m_transform->m_size.SetValue(630.0f, 50.0f, 1);
        selector->m_transform->m_color.SetValue(256.0f, 256.0f, 256.0f, 1.0f);
        auto comselect = selector->AddComponent<ComTitleSelectorMove>();
        comselect->m_widthMaxPlusSize = 30.0f;
        comselect->m_plusSizePerSecond = 30.0f;
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
        GameObject* datamanager = GameObject::Find("DataManager");
        if (datamanager == nullptr)
        {
            datamanager = GameObject::MakeNewObject("DataManager", E_TYPE_OBJECT::SYSTEM);
            datamanager->AddComponent<ComDataManager>();
        }
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
        //comcamera->m_offsetPos.SetValue(0, 30, 0);
    }
}

void CSceneTitle::Uninit()
{
}

void CSceneTitle::Update()
{
    {
        auto title = GameObject::Find("title");
        title->m_transform->m_color.AddValue(0, 0, 0, 0.001);
    }

    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
    {
        CSceneManager::GetInstance().LoadScene("SceneGame1");
    }
}

void CSceneTitle::Draw()
{
}