/**
 * @file CAssimpAnimationData.cpp
 * @author jupiter ()
 * @brief CAssimpAnimationDataの実装が記載されたcpp
 */

#include <windows.h>
#include "CAssimpAnimationData.h"

bool CAssimpAnimationData::Init(std::string filename)
{

    bool sts = m_assimpscene.Init(filename);
    if (!sts)
    {
        MessageBox(nullptr, "SkinAnimationDataAssimp load error", "error", MB_OK);
        return false;
    }

    return true;
}

void CAssimpAnimationData::Uninit()
{
    m_assimpscene.Exit();
}