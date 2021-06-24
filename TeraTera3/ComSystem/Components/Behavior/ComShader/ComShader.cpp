/**
 * @file ComShader.cpp
 * @author jupiter
 * @brief ComShaderの処理の一部が書かれたcpp
 * @version 1.0
 * @date 2020-08-07
 */
#include "ComShader.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"

using namespace std;

unordered_map<string, ID3D11VertexShader *> ComShader::m_pVertexShaders;

unordered_map<string, ID3D11PixelShader *> ComShader::m_pPixelShaders;

unordered_map<string, ID3D11InputLayout *> ComShader::m_pLayout;

int ComShader::m_classCount = 0;

void ComShader::Init()
{
    m_typeComponent.SetValue(E_TYPE_COMPONENT::SYSTEM);

    //デフォルト処理
    if (m_keyVertexShader.empty())
    {
        // 頂点データの定義 頂点フォーマットの順番
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

        unsigned int numElements = ARRAYSIZE(layout);

        LoadVertexShader("VS.fx", layout, numElements, true);
    }

    if (m_keyPixelShader.empty())
    {
        LoadPixelShader("PSLambert.fx", true);
    }

    CContainer::GetInstance().MakePermissionToList(this, "vertexshader");
    CContainer::GetInstance().MakePermissionToList(this, "pixelshader");
}

//================================================================================================
//================================================================================================

void ComShader::Uninit()
{

    m_classCount--;
    if (m_classCount <= 0)
    {
        if (m_pVertexShaders.empty() == false)
        {
            m_pVertexShaders.clear();
        }
        if (m_pPixelShaders.empty() == false)
        {
            m_pPixelShaders.clear();
        }
        if (m_pLayout.empty() == false)
        {
            m_pLayout.clear();
        }
    }
}

//================================================================================================
//================================================================================================

void ComShader::ImGuiDraw(unsigned int windowid)
{
    ImGui::BulletText("Use Pixel Shader : %s", m_keyPixelShader.c_str());
    ImGui::BulletText("Use Vertex Shader : %s", m_keyVertexShader.c_str());

    ImGui::BulletText("Pixel Shader Max : %d", m_pPixelShaders.size());

    if (ImGui::TreeNode("PixelShaders"))
    {
        for (auto &itr : m_pPixelShaders)
        {
            ImGui::BulletText(itr.first.c_str());
        }
        ImGui::TreePop();
    }

    ImGui::BulletText("Vertex Shader Max : %d", m_pPixelShaders.size());

    if (ImGui::TreeNode("VertexShaders"))
    {
        for (auto &itr : m_pPixelShaders)
        {
            ImGui::BulletText(itr.first.c_str());
        }
        ImGui::TreePop();
    }
}

//================================================================================================
//================================================================================================

void ComShader::LoadVertexShader(std::string _vsfile, D3D11_INPUT_ELEMENT_DESC *_layouts, unsigned int _layoutsize, bool _flag, std::string_view _foldername)
{

    std::string keyname = _foldername.data();
    keyname += "/" + _vsfile;

    //検索してまだ読み込まれていないなら通過
    if (!m_pVertexShaders.contains(keyname))
    {
        std::string filename = "Shader/" + keyname;

        ID3D11VertexShader *vertexshader;
        ID3D11InputLayout *layout;

        //存在しないので頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
        bool sts = CreateVertexShader(CDirectXGraphics::GetInstance().GetDXDevice(), //デバイス
                                      filename.c_str(),                              //シェーダーソースファイル
                                      "main",                                        //エントリー関数
                                      "vs_5_0",                                      //シェーダーモデル
                                      _layouts,                                      //頂点レイアウト
                                      _layoutsize,                                   //エレメント数
                                      &vertexshader,                                 //頂点シェーダーオブジェクト
                                      &layout);                                      //頂点レイアウト

        if (sts)
        {
            m_pLayout[keyname] = layout;
            m_pVertexShaders[keyname] = vertexshader;
        }
        else
        {
            MessageBox(nullptr, TEXT(filename.c_str()), TEXT("error"), MB_OK);
        }
    }

    if (_flag)
    {
        m_keyVertexShader = keyname;
    }
}

//================================================================================================
//================================================================================================

void ComShader::LoadPixelShader(std::string _psfile, bool _flag, std::string_view _foldername)
{

    std::string keyname = _foldername.data();
    keyname += "/" + _psfile;

    if (!m_pPixelShaders.contains(keyname))
    {
        std::string filename = "Shader/" + keyname;

        ID3D11PixelShader *pixelshader;

        //存在しないのでピクセルシェーダーを生成
        bool sts = CreatePixelShader(CDirectXGraphics::GetInstance().GetDXDevice(), // デバイス
                                     filename.c_str(),                              //ピクセルシェーダーファイル名
                                     "main",                                        //エントリー関数
                                     "ps_5_0",                                      //シェーダーモデル
                                     &pixelshader);                                 //ピクセルシェーダーオブジェクト
        if (sts)
        {
            m_pPixelShaders[keyname] = pixelshader;
        }
        else
        {
            MessageBox(nullptr, TEXT(filename.c_str()), TEXT("error"), MB_OK);
        }
    }
    if (_flag)
    {
        m_keyPixelShader = keyname;
    }
}

//================================================================================================
//================================================================================================

bool ComShader::ChangeVertexShader(std::string_view _shadername)
{
    //読み込み済みか？
    if (m_pVertexShaders.contains(_shadername.data()))
    {
        m_keyVertexShader = _shadername.data();
        return true;
    }
    return false;
}

//================================================================================================
//================================================================================================

void ComShader::SetVertexShader(std::string key)
{
    //頂点フォーマットをセット
    CDirectXGraphics::GetInstance().GetImmediateContext()->IASetInputLayout(m_pLayout[key]);

    //頂点シェーダーをセット
    CDirectXGraphics::GetInstance().GetImmediateContext()->VSSetShader(m_pVertexShaders[key], nullptr, 0);
}

//================================================================================================
//================================================================================================

void ComShader::SetPixelShader(std::string key)
{
    //ピクセルシェーダーをセット
    CDirectXGraphics::GetInstance().GetImmediateContext()->PSSetShader(m_pPixelShaders[key], nullptr, 0);
}
