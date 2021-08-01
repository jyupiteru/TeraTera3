/**
 * @file ComShader.cpp
 * @author jupiter
 * @brief ComShaderの処理の一部が書かれたcpp
 */
#include "ComShader.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../System/CDirectXGraphics/CDirectXGraphics.h"
#include "../../../../Managers/ShaderManager/CShaderManager.h"

using namespace std;

void ComShader::Init()
{
    m_typeComponent.SetValue(E_TYPE_COMPONENT::SYSTEM);

    //デフォルト処理
    if (CShaderManager::GetInstance().GetVertexShaderData("VS.fx") == nullptr)
    {
        // 頂点データの定義 頂点フォーマットの順番
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

        unsigned int numelements = ARRAYSIZE(layout);

        LoadVertexShader("VS.fx", layout, numelements, true);
    }

    ChangeVertexShader("VS.fx");
    LoadPixelShader("PSLambert.fx", true);
}

//================================================================================================
//================================================================================================

void ComShader::Uninit()
{
}

//================================================================================================
//================================================================================================

void ComShader::ImGuiDraw(unsigned int windowid)
{
    ImGui::BulletText("Use Pixel Shader : %s", m_keyPixelShader.c_str());
    ImGui::BulletText("Use Vertex Shader : %s", m_keyVertexShader.c_str());
}

//================================================================================================
//================================================================================================

void ComShader::LoadVertexShader(std::string _vsfile, D3D11_INPUT_ELEMENT_DESC *_layouts, unsigned int _layoutsize, bool _flag, std::string_view _foldername)
{

    CShaderManager::GetInstance().LoadVertexShader(_vsfile, _layouts, _layoutsize, _foldername);

    if (_flag)
    {
        std::string keyname = _foldername.data();
        keyname += "/";
        keyname += _vsfile.data();

        m_keyVertexShader = keyname;
    }
}

//================================================================================================
//================================================================================================

void ComShader::LoadPixelShader(std::string _psfile, bool _flag, std::string_view _foldername)
{

    CShaderManager::GetInstance().LoadPixelShader(_psfile, _foldername);

    if (_flag)
    {
        std::string keyname = _foldername.data();
        keyname += "/";
        keyname += _psfile.data();

        m_keyPixelShader = keyname;
    }
}

//================================================================================================
//================================================================================================

bool ComShader::ChangeVertexShader(std::string_view _shadername, std::string_view _foldername)
{

    //読み込み済みか？
    if (CShaderManager::GetInstance().GetVertexShaderData(_shadername, _foldername) != nullptr)
    {
        std::string keyname = _foldername.data();
        keyname += "/";
        keyname += _shadername.data();

        m_keyVertexShader = keyname.data();
        return true;
    }

    return false;
}

//================================================================================================
//================================================================================================

void ComShader::SetVertexShader()
{
    CShaderManager::GetInstance().SetVertexShader(m_keyVertexShader);
}

//================================================================================================
//================================================================================================

void ComShader::SetVertexShader(std::string key)
{
    CShaderManager::GetInstance().SetVertexShader(key);
}

//================================================================================================
//================================================================================================

void ComShader::SetPixelShader()
{
    CShaderManager::GetInstance().SetPixelShader(m_keyPixelShader);
}

//================================================================================================
//================================================================================================

void ComShader::SetPixelShader(std::string key)
{
    CShaderManager::GetInstance().SetPixelShader(key);
}