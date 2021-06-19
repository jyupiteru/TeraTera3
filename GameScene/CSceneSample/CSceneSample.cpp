#include "CSceneSample.h"

#include "../../GameComponent/Tests/Test.h"
#include "../../GameComponent/Tests/Test2.h"
#include "../../TeraTera3/TeraTera.h"

void CSceneSample::Init()
{
    CDebugLog::GetInstance().Draw("init");
    { //ステージ（ドーム表示）
        auto skydome = GameObject::MakeNewObject("skydome", E_TYPE_OBJECT::NONE);
        skydome->AddPackage<Package3DModelObject>();
        skydome->GetComponent<Com3DModel>()->LoadModelData("skydome.x.dat");
        skydome->GetComponent<ComShader>()->LoadPixelShader("PSOnlyTex.fx", true);
        skydome->m_transform->m_size.SetValue(10, 10, 10);
    }

    //{
    //    auto ui = GameObject::MakeNewObject("ui", E_TYPE_OBJECT::UI);
    //    // ui->GetComponent<Com2DTexture>()->LoadTexture("airplane.png");
    //    ui->m_transform->m_worldPosition.SetValue(0, -SCREEN_HEIGHT / 2, 0);
    //    ui->m_transform->m_size.SetValue(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 1);
    //    ui->m_transform->m_angle.SetValue(50, 50, 50);
    //    //ui->m_transform->m_angle.SetValue(45, 45, 45);//ここまだ反映されない
    //    ui->m_transform->m_color.SetValue(256, 0, 0, 1.0f);
    //    ui->AddComponent<Test>()->FirstSet();
    //}

    //{
    //    auto ui2 = GameObject::MakeNewObject("ui2", E_TYPE_OBJECT::UI);
    //    // ui->GetComponent<Com2DTexture>()->LoadTexture("airplane.png");
    //    ui2->m_transform->m_worldPosition.SetValue(0, SCREEN_HEIGHT / 4, 0);
    //    ui2->m_transform->m_size.SetValue(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 1);
    //    ui2->m_transform->m_color.SetValue(0, 256, 0, 0.5f);
    //    ui2->m_transform->m_vector.SetValue(0, -1.0f, 0);
    //    ui2->AddComponent<Test>()->FirstSet();
    //}

    {
        auto chara = GameObject::MakeNewObject("chara", E_TYPE_OBJECT::UI);
        chara->m_activeFlag.SetValue(false);
        chara->RemoveComponent<Com2DTexture>();
        chara->m_transform->m_worldPosition.SetValue(0, 0, 0);
        chara->m_transform->m_size.SetValue(100, 100, 1);
        chara->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
        auto text = chara->AddComponent<Com2DText>();
        text->m_text = "A12345";
    }

    //{
    //    auto bill = GameObject::MakeNewObject("bill", E_TYPE_OBJECT::BILLBOARD);
    //    //bill->GetComponent<Com3DBillBoard>()->LoadTexture("UI64x64.png");
    //    bill->m_transform->m_worldPosition.SetValue(0, 0, 200);
    //    bill->m_transform->m_angle.SetValue(0, 0, 0);
    //    bill->m_transform->m_size.SetValue(200, 100, 100);
    //    bill->m_transform->m_color.SetValue(256, 0, 256, 1.0f);

    //    //透過処理のない画像の透過方法 colorの4つめを引く+実行優先順位をほかのものより後に(前なら何も表示されない)
    //    bill->m_transform->m_color.AddValue(0, 0, 0, -0.5);
    //    bill->AddComponent<Test>()->FirstSet();
    //}

    { //視点決定用
        auto model = GameObject::MakeNewObject("model", E_TYPE_OBJECT::MODEL3D);
        model->m_transform->m_worldPosition.SetValue(0, -30, -100);
        model->m_transform->m_size.SetValue(2, 2, 2);
        model->m_transform->m_angle.SetValue(0, 0, 0);
        model->GetComponent<Com3DModelAssimp>()->LoadModelData("Hal/hal.pmx", "HAL/");
        model->GetComponent<Com3DModelAssimp>()->m_firstAngle.SetValue(0, 90, 0);
    }
    //{
    //    auto Sphere = GameObject::MakeNewObject("Sphere", E_TYPE_OBJECT::MODEL3D);
    //    Sphere->RemoveComponent<Com3DModelAssimp>();
    //    Sphere->AddComponent<ComSphere>();
    //    Sphere->m_transform->m_worldPosition.SetValue(40, 0, 100);
    //    Sphere->m_transform->m_size.SetValue(100, 100, 100);
    //    Sphere->m_transform->m_color.SetValue(0, 256, 0, 0.3f);
    //    // box->m_transform->m_angle.SetValue(0, 0, 0);
    //}
    //{
    //    auto box = GameObject::MakeNewObject("box", E_TYPE_OBJECT::MODEL3D);
    //    box->RemoveComponent<Com3DModelAssimp>();
    //    box->AddComponent<ComBox>();
    //    box->m_transform->m_worldPosition.SetValue(0, 0, 100);
    //    box->m_transform->m_size.SetValue(100, 100, 100);
    //    box->m_transform->m_color.SetValue(256, 0, 0, 0.1f);
    //    // box->m_transform->m_angle.SetValue(0, 0, 0);
    //}
    //{
    //    auto model2 = GameObject::MakeNewObject("model2", E_TYPE_OBJECT::MODEL3D);
    //    model2->m_transform->m_worldPosition.SetValue(0, 0, 100);
    //    model2->m_transform->m_vector.SetValue(-0.5f, 0, 0);
    //    model2->m_transform->m_size.SetValue(100, 100, 100);
    //    model2->m_transform->m_angle.SetValue(0, 0, 0);
    //    model2->GetComponent<Com3DModelAssimp>()->LoadModelData("Hal/hal.pmx", "HAL/");
    //    model2->AddComponent<Test2>();

    //    auto collider = model2->AddComponent<ComBoxCollider3D>();
    //    //collider->m_draw = true;
    //    collider->m_isFirstJustSize = true;
    //    collider->m_isTrigger.SetValue(true);
    //    //collider->m_offsetAngle.SetValue(0, 45, 0);
    //    //collider->m_offsetPosition.SetValue(0, 30, 0);
    //}
    { //1体目のアニメーションモデル
        auto model2 = GameObject::MakeNewObject("animmodel", E_TYPE_OBJECT::MODEL3D);
        model2->m_transform->m_worldPosition.SetValue(300, -30, 100);
        model2->m_transform->m_size.SetValue(100, 100, 100);
        model2->m_transform->m_angle.SetValue(0, 0, 0);
        model2->m_transform->m_vector.SetValue(-1, 0, 0);
        model2->m_transform->m_color.SetValue(0, 100, 0, 0.5f);
        model2->AddComponent<Test2>();
        auto assimp = model2->GetComponent<Com3DModelAssimp>();
        assimp->LoadModelData("Player/idle_run.fbx", "Player/");

        auto collider = model2->AddComponent<ComBoxCollider3D>();
        //collider->m_draw = true;
        collider->m_isFirstJustSize = true;
        collider->m_isTrigger.SetValue(true);
        collider->m_draw = true;
    }

    { //1体目のアニメーションモデル
        auto model2 = GameObject::MakeNewObject("animmodel3", E_TYPE_OBJECT::MODEL3D);
        model2->m_transform->m_worldPosition.SetValue(0, -30, -100);
        model2->m_transform->m_size.SetValue(100, 100, 100);
        model2->m_transform->m_angle.SetValue(0, 0, 0);
        //model2->m_transform->m_vector.SetValue(-1, 0, 0);
        model2->AddComponent<Test2>();
        auto assimp = model2->GetComponent<Com3DModelAssimp>();
        assimp->LoadModelData("Player/idle_run.fbx", "Player/");

        auto collider = model2->AddComponent<ComBoxCollider3D>();
        //collider->m_draw = true;
        collider->m_isFirstJustSize = true;
        collider->m_isTrigger.SetValue(true);
        collider->m_draw = true;
    }

     { //2体目のアニメーションモデル
         auto model2 = GameObject::MakeNewObject("animmodel2", E_TYPE_OBJECT::MODEL3D);
         model2->m_transform->m_worldPosition.SetValue(-100, 50, 100);
         model2->m_transform->m_size.SetValue(100, 100, 100);
         model2->m_transform->m_angle.SetValue(0, 0, 0);
         model2->AddComponent<Test2>();
         auto assimp = model2->GetComponent<Com3DModelAssimp>();
         assimp->LoadModelData("Player/idle_run.fbx", "Player/");
         Com3DAnimationAssimp *anim = model2->AddComponent<Com3DAnimationAssimp>();
         anim->LoadAnimation("test1", "Player/idle_run.fbx");
         anim->ChangeAnimation("test1", 1);
         auto collider = model2->AddComponent<ComBoxCollider3D>();
         collider->m_draw = true;
         collider->m_isFirstJustSize = true;
         collider->m_color.SetValue(256, 256, 0, 0.5f);
         collider->m_offsetSize.SetValue(0, 50, 0);
     }

    //{
    //    auto colliderobject = GameObject::MakeNewObject("colliderobj", E_TYPE_OBJECT::MODEL3D);

    //    colliderobject->RemoveComponent<Com3DModelAssimp>();
    //    colliderobject->AddComponent<ComBox>();
    //    colliderobject->m_transform->m_worldPosition.SetValue(-100, 50, 100);
    //    colliderobject->m_transform->m_size.SetValue(100, 100, 100);
    //    colliderobject->m_transform->m_color.SetValue(0, 256, 0, 0.5f);
    //    colliderobject->m_transform->m_angle.SetValue(0, 45, 45);
    //    auto collider = colliderobject->AddComponent<ComBoxCollider3D>();
    //    collider->m_draw = true;
    //    collider->m_isFirstJustSize = true;
    //    collider->m_offsetSize.SetValue(20, 20, 20); //少し大きくしている
    //    collider->m_isTrigger.SetValue(true);
    //    collider->m_color.AddValue(-50, 0, 0, 0);
    //    // box->m_transform->m_angle.SetValue(0, 0, 0);

    //    //box2->m_activeFlag.SetValue(false);
    //}
    //{
    //    auto box2 = GameObject::MakeNewObject("box2", E_TYPE_OBJECT::MODEL3D);
    //    box2->RemoveComponent<Com3DModelAssimp>();
    //    box2->AddComponent<ComBox>()->m_enable.SetValue(true);
    //    box2->m_transform->m_worldPosition.SetValue(-100, 0, 100);
    //    box2->m_transform->m_size.SetValue(100, 100, 100);
    //    box2->m_transform->m_color.SetValue(0, 256, 0, 0.5f);
    //    auto collider = box2->AddComponent<ComBoxCollider3D>();
    //    collider->m_draw = false;
    //    collider->m_isFirstJustSize = true;
    //    collider->m_offsetSize.SetValue(20, 20, 20); //少し大きくしている
    //    collider->m_isTrigger.SetValue(true);
    //    // box->m_transform->m_angle.SetValue(0, 0, 0);

    //    //box2->m_activeFlag.SetValue(false);
    //}
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
    //{
    //    auto test = GameObject::MakeNewObject("test", E_TYPE_OBJECT::NONE);
    //    test->AddComponent<Test2>()->MakeChildObject(0, 1);
    //}
    //{
    //    auto test2 = GameObject::MakeNewObject("test2", E_TYPE_OBJECT::NONE);
    //    // test2->AddComponent<Test2>()->MakeChildObject(0, 0);
    //}
    float time =  CTimer::GetInstance().GetProgressTime();
    CDebugLog::GetInstance().Draw("init"+ std::to_string(time));
}

void CSceneSample::Uninit()
{
}

void CSceneSample::Update()
{
    auto camera = GameObject::Find("camera");
    auto obj = GameObject::Find("model");

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
        CSceneManager::GetInstance().LoadScene("SceneSample2");
        //GameObject::Find("ui")->GetComponent<Test>()->Reset();
        //GameObject::Find("ui3")->m_transform->m_size.AddValue(1, 1, 1);
        //GameObject::Find("model2")->m_transform->m_angle.AddValue(1, 1, 0);
    }
    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LSHIFT))
    {
        auto obj = GameObject::Find("chara");
        obj->m_activeFlag.SetValue(!obj->m_activeFlag.GetValue());
    }

    //if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
    //{
    //    GameObject::Find("ui3")->m_transform->m_angle.AddValue(0, 0, 1);
    //}
}

void CSceneSample::Draw()
{
}