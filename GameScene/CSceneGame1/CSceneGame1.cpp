#include "CSceneGame1.h"

void CSceneGame1::Init()
{
#ifndef _DEBUG
    CDebugLog::GetInstance().m_flagSurvival.SetValue(false);
    CImGuiManager::GetInstance().m_flagSurvival.SetValue(false);
#endif // DEBUG

    CDebugLog::GetInstance().Draw("init");

    { //ステージ（ドーム表示）
        if (GameObject *skydome = GameObject::Find("skydome"); skydome == nullptr)
        {
            skydome = GameObject::MakeNewObject("skydome", E_TYPE_OBJECT::NONE);
            skydome->AddPackage<Package3DModelObject>();
            skydome->GetComponent<Com3DModel>()->LoadModelData("skydome.x.dat");
            skydome->GetComponent<ComShader>()->LoadPixelShader("psskydome.fx", true);
            skydome->m_transform->m_worldPosition.SetValue(0, -500, 0);
            skydome->m_transform->m_size.SetValue(1, 1, 1);
            skydome->DontDestroyOnLoad();
            skydome->m_drawLayer.SetValue(-1);
        }
    }

    {
        if (GameObject *player = GameObject::Find("player"); player == nullptr)
        {
            player = GameObject::MakeNewObject("player", E_TYPE_OBJECT::MODEL3D);
            player->m_transform->m_worldPosition.SetValue(0, 50, 0);
            player->m_transform->m_size.SetValue(10, 10, 10);
            player->AddComponent<ComPlayer>();
            player->GetComponent<Com3DModelAssimp>()->LoadModelData("Player/Running.fbx", "Player/");
            auto collider = player->AddComponent<ComBoxCollider3D>();
            collider->m_isTrigger.SetValue(true);
            collider->m_isFirstJustSize = true;
            collider->m_draw = true;
            //player->GetComponent<Com3DModelAssimp>()->m_firstAngle.SetValue(0, 90, -90);
        }
    }

    {
        auto camera = GameObject::Find("camera");
        auto comcamera = camera->GetComponent<ComCamera>();
        comcamera->m_typeFixed = E_TYPE_FIXED::LOOKAT;
        //camera->GetComponent<ComCamera>()->m_otherTransform = GameObject::Find("player")->m_transform;
        comcamera->m_range.SetValue(30);
        comcamera->m_firstAngle.SetValue(60, 0, 0);
        comcamera->m_offsetPos.SetValue(0, 0, 0);
        camera->m_objectUpdatePriority.SetValue(-1);
    }

    {
        auto gamemanager = GameObject::MakeNewObject("GameManager", E_TYPE_OBJECT::NONE);
        gamemanager->AddComponent<ComGameManager>();
        auto mapmanager = gamemanager->AddComponent<ComMapManager>();
        mapmanager->m_MaphalfSize.SetValue(10);
        mapmanager->m_mapMax.SetValue(10, 10);
        mapmanager->CreateMap(0);

        auto shotmanager = gamemanager->AddComponent<ComShotManager>();
        shotmanager->m_intervalTime.SetValue(3.0f, 0.2f);
        shotmanager->m_shotRandNum.SetValue(0, 2, 4);
        shotmanager->m_shotSpeed.SetValue(4, 7, 10);

        shotmanager->m_shotSize.SetValue(5, 7, 10);
    }

    {
        auto timer = GameObject::MakeNewObject("Timer", E_TYPE_OBJECT::SYSTEM);
        timer->m_transform->m_worldPosition.SetValue(-SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 - 20.0f, 0.0f);
        timer->m_transform->m_size.SetValue(50.0f, 80.0f, 1.0f);
        timer->m_transform->m_color.SetValue(0.0f, 0.0f, 0.0f, 1.0f);
        auto comtimer = timer->AddComponent<ComTimer>();
        comtimer->m_maxTimeCount.SetValue(60);
    }
}

void CSceneGame1::Uninit()
{
}

void CSceneGame1::Update()
{
    GameObject *camera = GameObject::Find("camera");
    camera->m_transform->m_worldPosition.SetValue(GameObject::Find("player")->m_transform->m_worldPosition.GetValue());
}

void CSceneGame1::Draw()
{
}