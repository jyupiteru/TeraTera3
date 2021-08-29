
#include "CSceneResult.h"

void CSceneResult::Init()
{
    {
        auto camera = GameObject::Find("Camera");
        camera->GetComponent<ComCamera>()->m_typeFixed = E_TYPE_FIXED::MODELLOOKAT;
        camera->GetComponent<ComCamera>()->m_range.SetValue(100);
        camera->GetComponent<ComCamera>()->m_firstAngle.SetValue(15, 0, 0);
        camera->GetComponent<ComCamera>()->m_offsetPos.SetValue(0, 30, 0);
    }
    {
        auto ui = GameObject::MakeNewObject("ui", E_TYPE_OBJECT::NONE);
        ui->AddComponent<Com2DText>();
        ui->AddComponent<ComShader>();
        ui->GetComponent<Com2DText>()->m_text = "Thank you For Playing!!!";
        ui->m_transform->m_color.SetValue(0,0,0,1.0f);
        ui->m_transform->m_size.SetValue(600.0f, 50.0f, 1.0f);
    }

    {
        auto ui = GameObject::MakeNewObject("ui2", E_TYPE_OBJECT::NONE);
        ui->m_transform->m_worldPosition.AddValue(0, -100, 0);
        ui->AddComponent<Com2DText>();
        ui->AddComponent<ComShader>();
        ui->GetComponent<Com2DText>()->m_text = "Press ENTER to Back Title";
        ui->m_transform->m_color.SetValue(0, 0, 0, 1.0f);
        ui->m_transform->m_size.SetValue(600.0f, 50.0f, 1.0f);
    }
}

void CSceneResult::Uninit()
{
    //一時的な(裏技)削除方法 これのためにコンポーネント作成するのはもったいないので
    auto player = GameObject::Find("player");
    if (player != nullptr)
    {
        player->m_transform->Destroy(player);
    }
}

void CSceneResult::Update()
{
    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
    {
        CSceneManager::GetInstance().LoadScene("SceneMenu");
    }
}

void CSceneResult::Draw()
{
}