#include "CSceneGame1.h"

void CSceneGame1::Init()
{

    { //ステージ（ドーム表示）
        if (GameObject *skydome = GameObject::Find("skydome"); skydome == nullptr)
        {
            skydome = GameObject::MakeNewObject("skydome", E_TYPE_OBJECT::NONE);
            skydome->AddPackage<Package3DModelObject>();
            skydome->GetComponent<Com3DModel>()->LoadModelData("skydome.x.dat");
            skydome->GetComponent<ComShader>()->LoadPixelShader("PSOnlyTex.fx", true);
            skydome->m_transform->m_size.SetValue(10, 10, 10);
            skydome->DontDestroyOnLoad();
            skydome->m_drawLayer.SetValue(-1);
        }
    }

    {
        GameObject *player = GameObject::Find("player");
        if (player == nullptr)
        {
            player = GameObject::MakeNewObject("player", E_TYPE_OBJECT::MODEL3D);
        }
        player->m_transform->m_worldPosition.SetValue(0, 50, -30);
        player->m_transform->m_size.SetValue(8, 8, 8);
        player->GetComponent<Com3DModelAssimp>()->LoadModelData("Player/idle_run.fbx", "Player/");
        auto move = player->AddComponent<ComPlayerMove>();
        player->DontDestroyOnLoad();
        move->m_moveSpeed = 14.0f;

        Com3DAnimationAssimp *anim = player->AddComponent<Com3DAnimationAssimp>();

        anim->ChangeAnimation("idle");

        ComBoxCollider3D *collider = player->AddComponent<ComBoxCollider3D>();
        collider->m_isTrigger.SetValue(true);
        collider->m_isFirstJustSize = true;
        collider->m_offsetSize.SetValue(-5.0f, 1.3f, -5.0f);
    }

    {
        auto camera = GameObject::Find("Camera");
        camera->GetComponent<ComCamera>()->m_typeFixed = E_TYPE_FIXED::LOOKAT;
        camera->GetComponent<ComCamera>()->m_range.SetValue(15);
        camera->GetComponent<ComCamera>()->m_firstAngle.SetValue(40, 0, 0);
        camera->GetComponent<ComCamera>()->m_offsetPos.SetValue(0, 0, 0);
        camera->AddComponent<ComCameraCustom>();
        camera->m_objectUpdatePriority.SetValue(-1);
    }

    {
        auto mapmanager = GameObject::Find("MapManager");
        if (mapmanager == nullptr)
        {
            mapmanager = GameObject::MakeNewObject("MapManager", E_TYPE_OBJECT::SYSTEM);
            mapmanager->AddComponent<ComMapManager>();
            mapmanager->AddComponent<ComMapMaker>();
        }
        mapmanager->GetComponent<ComMapMaker>()->MakeMap(1);
    }

    {
        auto timer = GameObject::MakeNewObject("timer", E_TYPE_OBJECT::SYSTEM);
        timer->m_transform->m_worldPosition.SetValue(-SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 - 20.0f, 0.0f);
        timer->m_transform->m_size.SetValue(50.0f, 80.0f, 1.0f);
        timer->m_transform->m_color.SetValue(0.0f, 0.0f, 0.0f, 1.0f);
        timer->AddComponent<ComTimer>()->m_maxCount = 60;
        timer->AddComponent<Com2DText>()->m_flagSetRate.SetValue(true);
    }
    {
        auto bottom = GameObject::MakeNewObject("buttom", E_TYPE_OBJECT::MODEL3D);
        bottom->m_transform->m_worldPosition.SetValue(0.0f, -14.0f, 0.0f);
        bottom->m_transform->m_size.SetValue(2000.0f, 1.0f, 2000.0f);
        bottom->m_transform->m_color.SetValue(0.0f, 0.0f, 0.0f, 0.0f);
        bottom->RemoveComponent<Com3DModelAssimp>();
        bottom->AddComponent<ComBox>();
        bottom->AddComponent<ComButtomBox>();
        auto collider = bottom->AddComponent<ComBoxCollider3D>();
        //collider->m_isFirstJustSize = true;
#ifdef _DEBUG
#endif // _DEBUG
    }
    {
        auto gamemanager = GameObject::MakeNewObject("GameManager", E_TYPE_OBJECT::SYSTEM);
        gamemanager->AddComponent<ComFlowManager>();
    }
}

void CSceneGame1::Uninit()
{
    {
        auto camera = GameObject::Find("Camera");
        camera->RemoveComponent<ComCameraCustom>();

    }
}

void CSceneGame1::Update()
{
    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
    {
        CSceneManager::GetInstance().LoadScene("SceneResult");
    }
}

void CSceneGame1::Draw()
{
}