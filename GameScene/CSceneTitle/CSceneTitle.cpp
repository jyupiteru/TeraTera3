#include "CSceneTitle.h"

void CSceneTitle::Init()
{
#ifndef _DEBUG
    CImGuiManager::GetInstance().m_flagSurvival.SetValue(false);
    CDebugLog::GetInstance().m_flagActive.SetValue(false);
#endif

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
        GameObject *player = GameObject::MakeNewObject("player", E_TYPE_OBJECT::MODEL3D);
        player->m_transform->m_worldPosition.SetValue(8.0f, 0.0f, -2.0f);
        player->m_transform->m_angle.SetValue(0.0f, 140.0f, 0.0f);
        player->m_transform->m_size.SetValue(8.0f, 8.0f, 8.0f);

        Com3DModelAssimp* commodel = player->GetComponent<Com3DModelAssimp>();
        commodel->LoadModelData("Player/idle_run.fbx", "Player/");
        //commodel->m_flagDrawShadow = true;

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
        auto floor = GameObject::MakeNewObject("Floor", E_TYPE_OBJECT::MODEL3D);
        floor->RemoveComponent<Com3DModelAssimp>();
        floor->AddComponent<ComBox>();
        floor->m_transform->m_size.SetValue(40.0f, 1.0f, 40.0f);
        floor->m_transform->m_worldPosition.SetValue(0.0f, -5.0f, 0.0f);
        floor->m_transform->m_color.AddValue(-100.0f, -100.0f, -100.0f, 0.0f);
    }

    { //カメラの取得と交信
        auto camera = GameObject::Find("Camera");
        camera->m_transform->m_worldPosition.SetValue(0, 0, 0);

        auto comcamera = camera->GetComponent<ComCamera>();
        comcamera->m_typeFixed = E_TYPE_FIXED::LOOKAT;
        //camera->GetComponent<ComCamera>()->m_otherTransform = GameObject::Find("player")->m_transform;
        comcamera->m_range.SetValue(20);
        comcamera->m_firstAngle.SetValue(45, 0, 0);
        comcamera->m_offsetPos.SetValue(0, 0, 0);
        camera->m_objectUpdatePriority.SetValue(-1);
    }
    {
        GameObject *datamanager = GameObject::Find("DataManager");
        if (datamanager == nullptr)
        {
            datamanager = GameObject::MakeNewObject("DataManager", E_TYPE_OBJECT::SYSTEM);
            datamanager->DontDestroyOnLoad();
            datamanager->AddComponent<ComDataManager>();
        }
        datamanager->GetComponent<ComDataManager>()->LoadScore();
    }
    {
        GameObject *titlemanager = GameObject::MakeNewObject("titlemanager", E_TYPE_OBJECT::SYSTEM);
        titlemanager->AddComponent<ComTitleManager>();
    }
    {
        GameObject *wipemanager = GameObject::Find("WipeManager");
        if (wipemanager == nullptr)
        {
            wipemanager = GameObject::MakeNewObject("WipeManager", E_TYPE_OBJECT::UI);
            wipemanager->m_transform->m_size.SetValue(SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            wipemanager->m_transform->m_color.SetValue(256, 256, 256, 1.0f);
            wipemanager->RemoveComponent<Com2DTexture>();
            wipemanager->AddComponent<ComWipe>();
            wipemanager->AddComponent<ComWipeManager>();
            wipemanager->DontDestroyOnLoad();
        }
    }
}

void CSceneTitle::Uninit()
{
}

void CSceneTitle::Update()
{
    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
    {
        //ゲームシーンへの遷移
        // CSceneManager::GetInstance().LoadScene("SceneGame1");
        ComWipeManager::GetInstance().StartWipe(E_TYPE_WIPE::GAME_WIPE_CLOSE);
    }
}

void CSceneTitle::Draw()
{
}