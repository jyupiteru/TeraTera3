#include "CSceneSample2.h"

#include "../../GameComponent/Tests/Test.h"
#include "../../GameComponent/Tests/Test2.h"
#include "../../TeraTera3/TeraTera.h"

void CSceneSample2::Init()
{
    { //ステージ（ドーム表示）
        auto skydome = GameObject::MakeNewObject("skydome", E_TYPE_OBJECT::NONE);
        skydome->m_drawLayer.SetValue(-1);
        skydome->AddPackage<Package3DModelObject>();
        skydome->GetComponent<Com3DModel>()->LoadModelData("skydome.x.dat");
        skydome->GetComponent<ComShader>()->LoadPixelShader("PSOnlyTex.fx", true);
        skydome->m_transform->m_size.SetValue(10, 10, 10);
    }

    {
        auto ui = GameObject::MakeNewObject("ui", E_TYPE_OBJECT::UI);
        // ui->GetComponent<Com2DTexture>()->LoadTexture("airplane.png");
        ui->m_transform->m_worldPosition.SetValue(0, -SCREEN_HEIGHT / 2, 0);
        ui->m_transform->m_size.SetValue(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 1);
        ui->m_transform->m_angle.SetValue(50, 50, 50);
        //ui->m_transform->m_angle.SetValue(45, 45, 45);//ここまだ反映されない
        ui->m_transform->m_color.SetValue(256, 0, 0, 1.0f);
        ui->AddComponent<Test>()->FirstSet();
    }

    {
        auto ui2 = GameObject::MakeNewObject("ui2", E_TYPE_OBJECT::UI);
        // ui->GetComponent<Com2DTexture>()->LoadTexture("airplane.png");
        ui2->m_transform->m_worldPosition.SetValue(0, SCREEN_HEIGHT / 4, 0);
        ui2->m_transform->m_size.SetValue(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 1);
        ui2->m_transform->m_color.SetValue(0, 256, 0, 0.5f);
        ui2->m_transform->m_vector.SetValue(0, -1.0f, 0);
        ui2->AddComponent<Test>()->FirstSet();
    }

    {
        auto chara = GameObject::MakeNewObject("chara", E_TYPE_OBJECT::UI);
        chara->RemoveComponent<Com2DTexture>();
        chara->m_transform->m_worldPosition.SetValue(0, 0, 0);
        chara->m_transform->m_size.SetValue(100, 100, 1);
        chara->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
        auto text = chara->AddComponent<Com2DText>();
        text->m_text = "A12345";
    }

    {
        auto bill = GameObject::MakeNewObject("bill", E_TYPE_OBJECT::BILLBOARD);
        //bill->GetComponent<Com3DBillBoard>()->LoadTexture("UI64x64.png");
        bill->m_transform->m_worldPosition.SetValue(0, 0, 200);
        bill->m_transform->m_angle.SetValue(0, 0, 0);
        bill->m_transform->m_size.SetValue(200, 100, 100);
        bill->m_transform->m_color.SetValue(256, 0, 256, 1.0f);

        //透過処理のない画像の透過方法 colorの4つめを引く+実行優先順位をほかのものより後に(前なら何も表示されない)
        bill->m_transform->m_color.AddValue(0, 0, 0, -0.5);
        bill->AddComponent<Test>()->FirstSet();
    }

    {
        auto model = GameObject::MakeNewObject("model", E_TYPE_OBJECT::MODEL3D);
        model->m_transform->m_worldPosition.SetValue(0, -30, -100);
        model->m_transform->m_size.SetValue(2, 2, 2);
        model->m_transform->m_angle.SetValue(0, 0, 0);
        model->GetComponent<Com3DModelAssimp>()->LoadModelData("Hal/hal.pmx", "HAL/");
        model->GetComponent<Com3DModelAssimp>()->m_firstAngle.SetValue(0, 90, 0);
    }
    {
        auto Sphere = GameObject::MakeNewObject("Sphere", E_TYPE_OBJECT::MODEL3D);
        Sphere->RemoveComponent<Com3DModelAssimp>();
        Sphere->AddComponent<ComSphere>();
        Sphere->m_transform->m_worldPosition.SetValue(40, 0, 100);
        Sphere->m_transform->m_size.SetValue(100, 100, 100);
        Sphere->m_transform->m_color.SetValue(256, 256, 256, 1.0f);
        Sphere->GetComponent<ComShader>()->LoadPixelShader("PSPointLight.fx", true);
        // box->m_transform->m_angle.SetValue(0, 0, 0);
    }
    {
        auto Sphere2 = GameObject::MakeNewObject("Sphere2", E_TYPE_OBJECT::MODEL3D);
        Sphere2->RemoveComponent<Com3DModelAssimp>();
        Sphere2->AddComponent<ComSphere>();
        Sphere2->m_transform->m_worldPosition.SetValue(140, 0, 100);
        Sphere2->m_transform->m_size.SetValue(100, 100, 100);
        Sphere2->m_transform->m_color.SetValue(256, 256, 256, 1.0f);
        Sphere2->GetComponent<ComShader>()->LoadPixelShader("PSPhong.fx", true);
    }
    {
        auto Sphere3 = GameObject::MakeNewObject("Sphere3", E_TYPE_OBJECT::MODEL3D);
        Sphere3->RemoveComponent<Com3DModelAssimp>();
        Sphere3->AddComponent<ComSphere>();
        Sphere3->m_transform->m_worldPosition.SetValue(240, 0, 100);
        Sphere3->m_transform->m_size.SetValue(100, 100, 100);
        Sphere3->m_transform->m_color.SetValue(256, 256, 256, 1.0f);
        Sphere3->GetComponent<ComShader>()->LoadPixelShader("PSAmbient.fx", true);
    }
    {
        auto Sphere4 = GameObject::MakeNewObject("Sphere4", E_TYPE_OBJECT::MODEL3D);
        Sphere4->RemoveComponent<Com3DModelAssimp>();
        Sphere4->AddComponent<ComSphere>();
        Sphere4->m_transform->m_worldPosition.SetValue(340, 0, 100);
        Sphere4->m_transform->m_size.SetValue(100, 100, 100);
        Sphere4->m_transform->m_color.SetValue(256, 256, 256, 1.0f);
        Sphere4->GetComponent<ComShader>()->LoadPixelShader("PSPointLight.fx", true);
    }
    //{
    //    auto box = GameObject::MakeNewObject("box", E_TYPE_OBJECT::MODEL3D);
    //    box->RemoveComponent<Com3DModelAssimp>();
    //    box->AddComponent<ComBox>();
    //    box->m_transform->m_worldPosition.SetValue(0, 0, 100);
    //    box->m_transform->m_size.SetValue(100, 100, 100);
    //    box->m_transform->m_color.SetValue(256, 0, 0, 0.1f);
    //    // box->m_transform->m_angle.SetValue(0, 0, 0);
    //}
    {
        auto model2 = GameObject::MakeNewObject("model2", E_TYPE_OBJECT::MODEL3D);
        model2->m_transform->m_worldPosition.SetValue(0, 0, 100);
        model2->m_transform->m_vector.SetValue(-0.5f, 0, 0);
        model2->m_transform->m_size.SetValue(100, 100, 100);
        model2->m_transform->m_angle.SetValue(0, 0, 0);
        model2->GetComponent<Com3DModelAssimp>()->LoadModelData("Hal/hal.pmx", "HAL/");
        model2->AddComponent<Test2>();

        auto collider = model2->AddComponent<ComBoxCollider3D>();
        collider->m_draw = true;
        collider->m_isFirstJustSize = true;
        collider->m_isTrigger.SetValue(true);
        //collider->m_offsetAngle.SetValue(0, 45, 0);
        //collider->m_offsetPosition.SetValue(0, 30, 0);
    }
    {
        auto model2 = GameObject::MakeNewObject("model3", E_TYPE_OBJECT::MODEL3D);
        model2->m_transform->m_worldPosition.SetValue(-100, 50, 100);
        model2->m_transform->m_size.SetValue(100, 100, 100);
        model2->m_transform->m_angle.SetValue(0, 0, 0);
        model2->GetComponent<Com3DModelAssimp>()->LoadModelData("Hal/hal.pmx", "HAL/");
        model2->AddComponent<Test2>();

        auto collider = model2->AddComponent<ComBoxCollider3D>();
        collider->m_draw = true;
        collider->m_isFirstJustSize = true;
        collider->m_isTrigger.SetValue(true);
    }
    {
        auto box2 = GameObject::MakeNewObject("box2", E_TYPE_OBJECT::MODEL3D);
        box2->RemoveComponent<Com3DModelAssimp>();
        box2->AddComponent<ComBox>()->m_enable.SetValue(true);
        box2->m_transform->m_worldPosition.SetValue(-100, 0, 100);
        box2->m_transform->m_size.SetValue(100, 100, 100);
        box2->m_transform->m_color.SetValue(0, 256, 0, 0.5f);
        auto collider = box2->AddComponent<ComBoxCollider3D>();
        collider->m_draw = true;
        collider->m_isFirstJustSize = true;
        collider->m_offsetSize.SetValue(20, 20, 20); //少し大きくしている
        collider->m_isTrigger.SetValue(true);
        collider->m_color.AddValue(-50, 0, 0, 0);
        // box->m_transform->m_angle.SetValue(0, 0, 0);

        //box2->m_activeFlag.SetValue(false);
    }
    {
        auto obj = GameObject::MakeNewObject("PointLight", E_TYPE_OBJECT::MODEL3D);
        obj->RemoveComponent<Com3DModelAssimp>();
        auto light = obj->AddComponent<ComLightPoint>();
        obj->m_transform->m_size.SetValue(50, 50, 50);
        light->m_range.SetValue(100.0f);
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
}

void CSceneSample2::Uninit()
{
}

void CSceneSample2::Update()
{
    auto camera = GameObject::Find("Camera");
    auto obj = GameObject::Find("PointLight");

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

    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
    {
        //シーンのロード
        CSceneManager::GetInstance().LoadScene("SceneSample");
        //GameObject::Find("ui")->GetComponent<Test>()->Reset();
        //GameObject::Find("ui3")->m_transform->m_size.AddValue(1, 1, 1);
        //GameObject::Find("model2")->m_transform->m_angle.AddValue(1, 1, 0);
    }
    //if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
    //{
    //    GameObject::Find("ui3")->m_transform->m_angle.AddValue(0, 0, 1);
    //}
}

void CSceneSample2::Draw()
{
}