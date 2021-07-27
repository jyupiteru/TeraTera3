#include "CSceneSample3.h"

#include "../../GameComponent/Tests/Test.h"
#include "../../GameComponent/Tests/Test2.h"
#include "../../TeraTera3/TeraTera.h"

void CSceneSample3::Init()
{

    CDebugLog::GetInstance().Draw("init");
    { //ステージ（ドーム表示）
        auto skydome = GameObject::MakeNewObject("skydome", E_TYPE_OBJECT::NONE);
        skydome->m_drawLayer.SetValue(-1);
        skydome->AddPackage<Package3DModelObject>();
        skydome->GetComponent<Com3DModel>()->LoadModelData("skydome.x.dat");
        skydome->GetComponent<ComShader>()->LoadPixelShader("PSOnlyTex.fx", true);
        skydome->m_transform->m_size.SetValue(10, 10, 10);
    }

    { //視点決定用
        auto model = GameObject::MakeNewObject("model", E_TYPE_OBJECT::MODEL3D);
        model->m_transform->m_worldPosition.SetValue(0, -30, -100);
        model->m_transform->m_size.SetValue(2, 2, 2);
        model->m_transform->m_angle.SetValue(0, 0, 0);
        model->GetComponent<Com3DModelAssimp>()->LoadModelData("Hal/hal.pmx", "HAL/");
        model->GetComponent<Com3DModelAssimp>()->m_firstAngle.SetValue(0, 90, 0);
    }

    {
        auto stage = GameObject::MakeNewObject("stage", E_TYPE_OBJECT::MODEL3D);
        stage->RemoveComponent<Com3DModelAssimp>();
        stage->AddComponent<ComBox>();
        stage->m_transform->m_worldPosition.SetValue(0, -30, 0);
        stage->m_transform->m_size.SetValue(200, 10, 200);
        stage->m_transform->m_color.SetValue(160, 160, 160, 1.0f);
    }

    { //1体目のアニメーションモデル
        auto model2 = GameObject::MakeNewObject("animmodel", E_TYPE_OBJECT::MODEL3D);
        model2->m_transform->m_worldPosition.SetValue(0, -20, 30);
        model2->m_transform->m_size.SetValue(20, 20, 20);
        model2->m_transform->m_angle.SetValue(0, 0, 0);
        //model2->m_transform->m_color.SetValue(160, 160, 160, 0.5f);
        model2->AddComponent<Test2>();
        auto assimp = model2->GetComponent<Com3DModelAssimp>();
        assimp->LoadModelData("Player/idle_run.fbx", "Player/");

        auto collider = model2->AddComponent<ComBoxCollider3D>();
        //collider->m_draw = true;
        collider->m_isFirstJustSize = true;
        collider->m_isTrigger.SetValue(true);
        collider->m_draw = true;
    }
    {
        auto Sphere = GameObject::MakeNewObject("Sphere", E_TYPE_OBJECT::MODEL3D);
        Sphere->RemoveComponent<Com3DModelAssimp>();
        Sphere->AddComponent<ComSphere>();
        Sphere->m_transform->m_worldPosition.SetValue(40, -10, 30);
        Sphere->m_transform->m_size.SetValue(20, 20, 20);
        Sphere->m_transform->m_color.SetValue(256, 256, 256, 1.0f);
        //Sphere2->GetComponent<ComShader>()->LoadPixelShader("PSPhong.fx", true);
    }

    //{ //1体目のアニメーションモデル
    //    auto model2 = GameObject::MakeNewObject("animmodel3", E_TYPE_OBJECT::MODEL3D);
    //    model2->m_transform->m_worldPosition.SetValue(0, -30, -100);
    //    model2->m_transform->m_size.SetValue(100, 100, 100);
    //    model2->m_transform->m_angle.SetValue(0, 0, 0);
    //    //model2->m_transform->m_vector.SetValue(-1, 0, 0);
    //    model2->AddComponent<Test2>();
    //    auto assimp = model2->GetComponent<Com3DModelAssimp>();
    //    assimp->LoadModelData("Player/idle_run.fbx", "Player/");

    //    auto collider = model2->AddComponent<ComBoxCollider3D>();
    //    //collider->m_draw = true;
    //    collider->m_isFirstJustSize = true;
    //    collider->m_isTrigger.SetValue(true);
    //    collider->m_draw = true;
    //}

    {
        auto shadowtexture = GameObject::MakeNewObject("ShadowTexture", E_TYPE_OBJECT::UI);
        shadowtexture->m_transform->m_worldPosition.SetValue((500 + 250) / 2, (300 + 150) / 2, 1);
        shadowtexture->m_transform->m_size.SetValue(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 1);
    }

    {
        auto camera = GameObject::Find("Camera");
        auto com = camera->GetComponent<ComCamera>()->m_typeFixed = E_TYPE_FIXED::MODELLOOKAT;
        camera->m_transform->m_worldPosition.SetValue(0, 0, 0);
        camera->m_transform->m_angle.SetValue(0, 0, 0);

        camera->GetComponent<ComCamera>()->m_range.SetValue(40);
        camera->GetComponent<ComCamera>()->m_firstAngle.SetValue(0, 0, 0);
        camera->GetComponent<ComCamera>()->m_otherTransform = GameObject::Find("model")->m_transform;
        camera->GetComponent<ComCamera>()->m_offsetPos.SetValue(0, 10, 0);
        camera->GetComponent<ComCamera>()->m_firstAngle.SetValue(0, 0, 0);
        //camera->GetComponent<ComCamera>()->SetOutPosition(XMFLOAT3( 0, 0, 100000));
    }
    float time = CTimer::GetInstance().GetProgressTime();
    CDebugLog::GetInstance().Draw("init" + std::to_string(time));
}

void CSceneSample3::Uninit()
{
}

void CSceneSample3::Update()
{
    auto camera = GameObject::Find("Camera");
    auto obj = GameObject::Find("model");
    auto light = GameObject::Find("Light");

    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
    {
        obj->m_transform->m_angle.AddValue(0, 1, 0);
    }
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
    {
        obj->m_transform->m_angle.AddValue(0, -1, 0);
    }
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
    {
        obj->m_transform->m_angle.AddValue(-1, 0, 0);
    }
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
    {
        obj->m_transform->m_angle.AddValue(1, 0, 0);
    }

    {

        obj->m_transform->m_vector.SetValue(0, 0, 0);
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UPARROW))
        {
            obj->m_transform->m_vector.SetValue(0, 1, 0);
        }
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWNARROW))
        {
            obj->m_transform->m_vector.SetValue(0, -1, 0);
        }
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHTARROW))
        {
            obj->m_transform->m_vector.SetValue(1, 0, 0);
        }
        if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFTARROW))
        {
            obj->m_transform->m_vector.SetValue(-1, 0, 0);
        }
    }

    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_1))
    {
        light->m_transform->m_worldPosition.AddValue(-1, 0, 0);
    }
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_2))
    {
        light->m_transform->m_worldPosition.AddValue(1, 0, 0);
    }
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_3))
    {
        light->m_transform->m_worldPosition.AddValue(0, 0, 1);
    }
    if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_4))
    {
        light->m_transform->m_worldPosition.AddValue(0, 0, -1);
    }

    GameObject::Find("ShadowTexture")->GetComponent<Com2DTexture>()->SetTextureKey("ShadowTexture");
}

void CSceneSample3::Draw()
{
}