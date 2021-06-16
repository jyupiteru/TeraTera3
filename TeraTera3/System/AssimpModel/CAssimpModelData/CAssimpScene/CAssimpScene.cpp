/**
 * @file CAssimpScene.cpp
 * @author jupiter ()
 * @brief CAssimpSceneクラスの実装を記載したcpp
 */

#include <assimp/cimport.h>
#include <assimp\postprocess.h>
#include <assimp/cimport.h>
#include <assimp\scene.h>

#include "CAssimpScene.h"

bool CAssimpScene::Init(std::string_view _filename)
{

    //モデル読み込み
    m_Scene = aiImportFile(_filename.data(),
                           aiProcess_ConvertToLeftHanded |
                               aiProcessPreset_TargetRealtime_MaxQuality);

    if (m_Scene == nullptr)
    {
        return false;
    }

    return true;
}

//================================================================================================
//================================================================================================

void CAssimpScene::Exit()
{
    aiReleaseImport(m_Scene);
}

//================================================================================================
//================================================================================================

const aiScene *CAssimpScene::GetScene() const
{
    return m_Scene;
}

///================================================================================================
//================================================================================================

bool CAssimpScene::HasAnimation()
{
    return m_Scene->HasAnimations();
}