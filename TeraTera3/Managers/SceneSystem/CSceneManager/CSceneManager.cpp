/**
 * @file CSceneManager.cpp
 * @author jupiter
 * @brief CSceneManagerが載っているcpp
 */
#include "CSceneManager.h"

#include "../../../Setup.h"
#include "../../../DebugLog/CDebugLog.h"
#include "../../../ComSystem/Core/ObjectGenerator.h"
#include "../../../../GameScene/SceneSetter.h"
#include "../../../../Thirdparty/ImGui/imgui.h"

#include "../CScene.h"

CSceneManager *CSceneManager::m_instance = nullptr;

CSceneManager::CSceneManager()
{
    m_pListScene = new std::unordered_map<std::string, std::unique_ptr<CScene>>();
    this->LoadSceneList();
}

//================================================================================================
//================================================================================================

CSceneManager::~CSceneManager()
{
    m_pNowScene->Uninit();

    //すべてのシーンの所有権を手放す
    for (auto itr = m_pListScene->begin(); itr != m_pListScene->end(); itr++)
    {
        itr->second.reset();
    }
    m_pListScene->clear();
    delete m_pListScene;
}

//================================================================================================
//================================================================================================

void CSceneManager::Update()
{
    //一括でのシーンの変更処理
    ChangeScene();

    m_pNowScene->Update();
    ObjectGenerator::GetInstance().Update();
}

//================================================================================================
//================================================================================================

void CSceneManager::Draw()
{
    m_pNowScene->Draw();
    ObjectGenerator::GetInstance().Draw();
}

//================================================================================================
//================================================================================================

void CSceneManager::Create()
{
    m_instance = new CSceneManager();
}

//================================================================================================
//================================================================================================

void CSceneManager::Delete(bool _flag)
{
    if (_flag)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

//================================================================================================
//================================================================================================

CSceneManager &CSceneManager::GetInstance()
{
    return *m_instance;
}

//================================================================================================
//================================================================================================

void CSceneManager::ImGuiDraw(unsigned int windowid)
{
    if (ImGui::TreeNode("SceneList"))
    {
        for (auto &itr : *m_pListScene)
        {
            ImGui::BulletText(itr.first.c_str());
        }
        ImGui::TreePop();
    }
}

//================================================================================================
//================================================================================================

bool CSceneManager::LoadScene(std::string_view scenename)
{
    //このシーン名は存在しているか
    if ((*m_pListScene).contains(scenename.data()))
    {
        m_nextScene = scenename.data();
        return true;
    }
    return false;
}

//================================================================================================
//================================================================================================
//private

void CSceneManager::LoadSceneList()
{
    SceneSetter *scene = new SceneSetter();
    m_pListScene->merge(*scene->GetSetScene());

    //最初のシーンをセットして読み込み
    LoadScene(scene->firstSceneName);
    ChangeScene();

    delete scene;
}

//================================================================================================
//================================================================================================

CScene *const CSceneManager::GetNextScene(std::string name)
{
    return (*m_pListScene)[name].get();
}

//================================================================================================
//================================================================================================

void CSceneManager::ChangeScene()
{
    if (m_nextScene != "None")
    {
        if (m_pNowScene != nullptr)
        {
            m_pNowScene->Uninit();
        }
        //次のシーンを呼び出し
        m_pNowScene = GetNextScene(m_nextScene);

        ObjectGenerator::GetInstance().ChangeScene(m_nextScene);

        m_pNowScene->Init();
        m_nextScene = "None";
    }
}