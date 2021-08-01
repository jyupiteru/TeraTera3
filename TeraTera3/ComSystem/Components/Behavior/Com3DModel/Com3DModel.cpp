/**
 * @file Com3DModel.cpp
 * @author jupiter
 * @brief Com3DModelの実装の一部が書かれたcpp
 */
#include "Com3DModel.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../System/DX11Settransform.h"

using namespace DirectX;

std::unordered_map<std::string, std::shared_ptr<DatFile>> Com3DModel::m_pListModel;

int Com3DModel::m_classCount = 0;

void Com3DModel::Init()
{
    m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT3D);
    m_classCount++;
}

//================================================================================================
//================================================================================================

void Com3DModel::Uninit()
{
    m_classCount--;
    if (m_classCount <= 0)
    {
        for (auto itr = m_pListModel.begin(); itr != m_pListModel.end(); itr++)
        {
            itr->second.reset();
        }
        m_pListModel.clear();
    }
}

//================================================================================================
//================================================================================================

void Com3DModel::Ready()
{
    m_pShader = m_gameObject->GetComponent<ComShader>();
    if (m_pShader == nullptr)
    {
        m_pShader = m_gameObject->AddComponent<ComShader>();
    }
}

//================================================================================================
//================================================================================================

void Com3DModel::Draw()
{
    m_pShader->SetPixelShader();
    m_pShader->SetVertexShader();
    XMFLOAT4X4 mtx = m_gameObject->m_transform->GetMatrix();
    DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);
    // モデル描画
    m_pListModel[m_keyModel]->Draw(CDirectXGraphics::GetInstance().GetImmediateContext());
}

//================================================================================================
//================================================================================================

void Com3DModel::ImGuiDraw(unsigned int windowid)
{
    ImGui::BulletText("ModelName : %s", m_keyModel.c_str());
    ImGui::Text("Here has nodisplay things");
}

//================================================================================================
//================================================================================================

void Com3DModel::LoadModelData(std::string modelname)
{
    modelname = "Assets/Models/" + modelname;
    m_keyModel = modelname;
    if (!m_pListModel.contains(m_keyModel))
    {
        std::shared_ptr<DatFile> dat = std::make_shared<DatFile>();
        bool sts = dat->Load(modelname.c_str(), CDirectXGraphics::GetInstance().GetDXDevice(), CDirectXGraphics::GetInstance().GetImmediateContext());
        if (sts)
        {
            m_pListModel[m_keyModel] = dat;
        }
        else
        {
            MessageBox(nullptr, TEXT(modelname.c_str()), TEXT("error"), MB_OK);
        }
    }
}