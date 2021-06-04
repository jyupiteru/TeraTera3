#include "ComShotManager.h"

ComShotManager *ComShotManager::m_instance = nullptr;

void ComShotManager::Init()
{
    m_instance = this;
    m_intervalTime.SetValue(4.0f);
    m_shotMax.SetValue(10);

    m_shotSpeed.SetValue(3.0f);
}

//================================================================================================
//================================================================================================

void ComShotManager::Uninit()
{
    m_listFirstPos.clear();
    m_listShotList.clear();
    m_listWaitCreate.clear();
    m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComShotManager::Ready()
{
    //同時生成可能数の3倍生成する
    for (int i = 0; i < m_shotMax.GetValue() * 3; i++)
    {
        //オブジェクトの生成
        std::string name = "Shot No" + std::to_string(i);
        GameObject *shot = m_gameObject->AddChildObject(name, E_TYPE_OBJECT::MODEL3D);

        //各コンポーネントの設定
        shot->RemoveComponent<Com3DModelAssimp>();
        shot->AddComponent<ComSphere>();
        shot->m_transform->m_color.SetValue(255, 0, 255, 1.0f);
        shot->m_activeFlag.SetValue(false);
        ComShot *comshot = shot->AddComponent<ComShot>();

        //リストへの格納
        m_listWaitCreate.emplace_back(comshot);
        m_listShotList.emplace_back(comshot);

        //衝突判定関係の設定
        ComBoxCollider3D *collider = shot->AddComponent<ComBoxCollider3D>();
        collider->m_isTrigger.SetValue(true);
        collider->m_draw = true;
        collider->m_isFirstJustSize = true;
    }
}

//================================================================================================
//================================================================================================

void ComShotManager::Update()
{
}

//================================================================================================
//================================================================================================

ComShotManager &ComShotManager::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void ComShotManager::SetShotReadyList(ComShot *_shot)
{
    _shot->m_gameObject->m_activeFlag.SetValue(false);
    m_listWaitCreate.push_back(_shot);
}

//================================================================================================
//================================================================================================

void ComShotManager::SetShotFirstPos(std::vector<std::pair<float, float>> &_firstpos)
{
    m_listFirstPos = _firstpos;
}