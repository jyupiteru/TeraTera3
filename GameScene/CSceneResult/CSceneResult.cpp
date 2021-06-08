#include "CSceneResult.h"

void CSceneResult::Init()
{
    { //ステージ（ドーム表示）
        if (GameObject* skydome = GameObject::Find("skydome"); skydome == nullptr)
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
    { //カメラの取得と交信
        auto camera = GameObject::Find("camera");
        auto comcamera = camera->GetComponent<ComCamera>();
        comcamera->m_typeFixed = E_TYPE_FIXED::LOOKAT;
        comcamera->m_range.SetValue(30);
        comcamera->m_firstAngle.SetValue(60, 0, 0);
        comcamera->m_offsetPos.SetValue(0, 0, 0);
        camera->m_objectUpdatePriority.SetValue(-1);
    }
    {
        auto chara = GameObject::MakeNewObject("result", E_TYPE_OBJECT::UI);
        chara->RemoveComponent<Com2DTexture>();
        chara->m_transform->m_worldPosition.SetValue(0, SCREEN_HEIGHT/4, 0);
        chara->m_transform->m_size.SetValue(300, 250, 1);
        chara->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
        auto text = chara->AddComponent<Com2DText>();
        text->m_text = "RESULT";
    }
    {
        auto chara = GameObject::MakeNewObject("chara", E_TYPE_OBJECT::UI);
        chara->RemoveComponent<Com2DTexture>();
        chara->m_transform->m_worldPosition.SetValue(0, 0, 0);
        chara->m_transform->m_size.SetValue(500, 250, 1);
        chara->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
        auto text = chara->AddComponent<Com2DText>();
        text->m_text = "Thank you For Playing";
    }
}

void CSceneResult::Uninit()
{
}

void CSceneResult::Update()
{
}

void CSceneResult::Draw()
{
}