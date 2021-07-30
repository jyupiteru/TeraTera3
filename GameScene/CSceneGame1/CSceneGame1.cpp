#include "CSceneGame1.h"

void CSceneGame1::Init()
{
#ifndef _DEBUG
    CDebugLog::GetInstance().m_flagActive.SetValue(false);
    CImGuiManager::GetInstance().m_flagSurvival.SetValue(false);
#endif // DEBUG

    CDebugLog::GetInstance().Draw("init");
    CShadowManager::GetInstance().m_flagActive.SetValue(true);

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

    { //プレイヤーの生成
        GameObject *player = GameObject::MakeNewObject("Player", E_TYPE_OBJECT::MODEL3D);
        player->m_transform->m_worldPosition.SetValue(0, 50, 0);
        player->m_transform->m_size.SetValue(10, 10, 10);
        player->AddComponent<ComPlayer>();
        player->GetComponent<Com3DModelAssimp>()->LoadModelData("Player/idle_run.fbx", "Player/");

        auto anim = player->AddComponent<Com3DAnimationAssimp>();

        if (!anim->ChangeAnimation("idle"))
        { //読み込めていないので各種アニメーション読み込み
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
            anim->ChangeAnimation("idle");
        }

        auto collider = player->AddComponent<ComBoxCollider3D>();
        collider->m_isTrigger.SetValue(true);
        collider->m_isFirstJustSize = true;
        collider->m_offsetSize.SetValue(-7.0f, 0.5f, -0.5f);

#ifdef _DEBUG
        collider->m_draw = true;
#endif // _DEBUG
    }

    { //カメラの取得と交信
        auto camera = GameObject::Find("Camera");
        auto comcamera = camera->GetComponent<ComCamera>();
        comcamera->m_typeFixed = E_TYPE_FIXED::LOOKAT;
        //camera->GetComponent<ComCamera>()->m_otherTransform = GameObject::Find("player")->m_transform;
        comcamera->m_range.SetValue(30);
        comcamera->m_firstAngle.SetValue(60, 0, 0);
        comcamera->m_offsetPos.SetValue(0, 0, 0);
        camera->m_objectUpdatePriority.SetValue(-1);
    }

    { //ゲームの流れを管理するクラスの生成
        auto gamemanager = GameObject::MakeNewObject("GameManager", E_TYPE_OBJECT::NONE);
        gamemanager->AddComponent<ComGameManager>();

        //マップを管理するコンポーネントの追加とマップの生成
        auto mapmanager = gamemanager->AddComponent<ComMapManager>();
        mapmanager->m_MaphalfSize.SetValue(10);
        mapmanager->m_mapMax.SetValue(10, 10);
        mapmanager->CreateMap(0);

        //弾を管理するコンポーネントの追加
        auto shotmanager = gamemanager->AddComponent<ComShotManager>();
        shotmanager->m_intervalTime.SetValue(2.0f, 0.1f);
        shotmanager->m_shotRandNum.SetValue(0, 3, 7);
        shotmanager->m_shotSpeed.SetValue(4, 7, 12);

        shotmanager->m_shotSize.SetValue(1.0f, 0.7f, 0.7f);
        shotmanager->m_shotSizeRate.SetValue(5, 7, 10);

        //コインを管理するコンポーネントの追加
        auto coinmanager = gamemanager->AddComponent<ComCoinManager>();
        coinmanager->m_coinScore.SetValue(100, 300);
        coinmanager->m_coinSize.SetValue(4, 4, 4);
        coinmanager->m_coinMax.SetValue(20);
        coinmanager->m_intervalTime.SetValue(2.0f, 0.1f);
    }

    { //時間を管理するオブジェクトの生成
        auto timer = GameObject::MakeNewObject("Timer", E_TYPE_OBJECT::SYSTEM);
        timer->m_transform->m_worldPosition.SetValue(-SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 - 20.0f, 0.0f);
        timer->m_transform->m_size.SetValue(50.0f, 80.0f, 1.0f);
        timer->m_transform->m_color.SetValue(0.0f, 0.0f, 0.0f, 1.0f);
        auto comtimer = timer->AddComponent<ComTimer>();
        comtimer->m_maxTimeCount.SetValue(60);
    }

    {
        GameObject *datamanager = GameObject::Find("DataManager");
        if (datamanager == nullptr)
        {
            datamanager = GameObject::MakeNewObject("DataManager", E_TYPE_OBJECT::SYSTEM);
            datamanager->DontDestroyOnLoad();
            datamanager->AddComponent<ComDataManager>();
        }
    }
    {
        GameObject* light = GameObject::Find("Light");
        light->m_transform->m_worldPosition.SetValue(0, 100, -10);
    }
}

void CSceneGame1::Uninit()
{
}

void CSceneGame1::Update()
{
    GameObject *camera = GameObject::Find("Camera");
    camera->m_transform->m_worldPosition.SetValue(GameObject::Find("Player")->m_transform->m_worldPosition.GetValue());
}

void CSceneGame1::Draw()
{
}