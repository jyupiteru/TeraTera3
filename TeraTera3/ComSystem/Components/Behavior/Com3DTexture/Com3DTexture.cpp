/**
 * @file Com3DTexture.cpp
 * @author jupiter
 * @brief Com3DTextureが記述されたcpp
 */
#include "Com3DTexture.h"

#include "../ComTransform/ComTransform.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../WindowsSystem/DX11Settransform.h"
#include "../../../../ImGuiSystem/ImGuiHelperFunctions.h"
#include "../../../../System/CTextureManager/CTextureManager.h"

using namespace DirectX;

// 定数バッファ定義（マテリアル単位）
struct ConstantBufferMaterial
{
    DirectX::XMFLOAT4 AmbientMaterial;  // 環境光の反射率
    DirectX::XMFLOAT4 DiffuseMaterial;  // ディフューズ光の反射率
    DirectX::XMFLOAT4 SpecularMaterial; // スペキュラ光の反射率
};

//================================================================================================
//================================================================================================

void Com3DTexture::Init()
{
    m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT3D);

    this->LoadTexture("System/white.png", E_TYPE_TEXTUREOBJ::VERTICAL);
}

//================================================================================================
//================================================================================================

void Com3DTexture::Uninit()
{
    if (m_screenBuffer)
    {
        m_screenBuffer->Release();
        m_screenBuffer = nullptr;
    }
}

//================================================================================================
//================================================================================================

void Com3DTexture::Ready()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

    unsigned int numElements = ARRAYSIZE(layout);

    m_pShader = m_gameObject->GetComponent<ComShader>();
    if (m_pShader == nullptr)
    {
        m_pShader = m_gameObject->AddComponent<ComShader>();
    }
    m_pShader->LoadVertexShader("VS3DTex.fx", layout, numElements, true);
    m_pShader->LoadPixelShader("PSTexWithColor.fx", true);

    if (auto [x, y, z] = m_gameObject->m_transform->m_size.GetValue(); z == 0.0f)
    {
        m_gameObject->m_transform->m_size.AddValue(0, 0, 1.0f);
    }
}

//================================================================================================
//================================================================================================

void Com3DTexture::Draw()
{

    SetVertex();
    m_pShader->SetVertexShader();

    m_pShader->SetPixelShader();

    XMFLOAT4X4 mat = this->m_gameObject->m_transform->GetMatrix();

    // ワールド変換行列
    DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mat);

    // イミィディエイトコンテキスト
    ID3D11DeviceContext *devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();

    // 頂点バッファをセット
    unsigned int stride = sizeof(tagVertex);
    unsigned int offset = 0;

    // 頂点バッファをデバイスコンテキストへセット
    devcontext->IASetVertexBuffers(
        0,               // スタートスロット
        1,               // 頂点バッファ個数
        &m_vertexbuffer, // 頂点バッファの先頭アドレス
        &stride,         // ストライド
        &offset);        // オフセット

    // トポロジーをセット
    devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // インデックスバッファをセット
    devcontext->IASetIndexBuffer(m_idxbuffer, DXGI_FORMAT_R32_UINT, 0);

    auto data = CTextureManager::GetInstance().GetTextureData(m_keyTexture);

    ID3D11ShaderResourceView *srv = data->srv;

    // PSにSRVをセット
    devcontext->PSSetShaderResources(
        0,     //t0レジスタ
        1,     //個数
        &srv); //SRV

    // PSに定数バッファをセット
    devcontext->PSSetConstantBuffers(
        3, //t3レジスタ
        1, //個数
        &m_cbuffer);

    // GPUコマンドを発行
    devcontext->DrawIndexed(4, 0, 0);
}

//================================================================================================
//================================================================================================

void Com3DTexture::ImGuiDraw(unsigned int windowid)
{
    ImGui::BulletText("TextureName : %s", m_keyTexture.c_str());

    ImGui::BulletText("TextureNum");
    ImGui::SameLine();
    HelpMarker((const char *)u8"現在の何個目のテクスチャか 左上を1,1とする");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f", m_textureNum.x, m_textureNum.y);
    ImGui::Unindent();

    ImGui::BulletText("TextureRate");
    ImGui::SameLine();
    HelpMarker((const char *)u8"テクスチャ１枚当たりの割合");
    ImGui::Indent();
    ImGui::Text("X : %0.1f, Y : %0.1f", m_textureRate.x, m_textureRate.y);
    ImGui::Unindent();
}

//================================================================================================
//================================================================================================

void Com3DTexture::LoadTexture(std::string texturename, E_TYPE_TEXTUREOBJ texturetype)
{
    m_typeTexture = texturetype;
    // インデックスバッファ生成
    unsigned int idx[4] = {0, 1, 2, 3};

    // インデックスバッファ生成
    bool sts = CreateIndexBuffer(CDirectXGraphics::GetInstance().GetDXDevice(), //デバイス
                                 4,                                             //インデックス数
                                 idx,                                           //初期化データの先頭アドレス
                                 &m_idxbuffer);                                 //インデックスバッファ
    if (!sts)
    {
        MessageBox(nullptr, TEXT("CreateIndexBuffer error"), TEXT("error"), MB_OK);
    }

    SetUV();

    // 定数バッファ生成
    sts = CreateConstantBufferWrite(CDirectXGraphics::GetInstance().GetDXDevice(), sizeof(ConstantBufferMaterial), &m_cbuffer);
    if (!sts)
    {
        MessageBox(nullptr, TEXT("CreateConstantBufferWrite error"), TEXT("error"), MB_OK);
    }

    m_keyTexture = texturename;

    //読み込んだことはあるか？
    if (!CTextureManager::GetInstance().GetTextureData(m_keyTexture))
    {
        CTextureManager::GetInstance().LoadTexture(m_keyTexture);
    }

    // マテリアル
    ConstantBufferMaterial material = {
        XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), //環境光マテリアル
        XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), //拡散反射マテリアル
        XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), //鏡面反射マテリアル
    };

    // 定数バッファ書き換え
    D3D11_MAPPED_SUBRESOURCE pData;
    HRESULT hr = CDirectXGraphics::GetInstance().GetImmediateContext()->Map(m_cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
    if (SUCCEEDED(hr))
    {
        memcpy_s(pData.pData, pData.RowPitch, (void *)(&material), sizeof(ConstantBufferMaterial));
        CDirectXGraphics::GetInstance().GetImmediateContext()->Unmap(m_cbuffer, 0);
    }
}

//================================================================================================
//================================================================================================

void Com3DTexture::SetVertex()
{

    switch (m_typeTexture)
    {
    case E_TYPE_TEXTUREOBJ::DEPTH:
        m_vertex[0].x = 0.0;
        m_vertex[0].y = 0.5;
        m_vertex[0].z = -0.5;

        m_vertex[1].x = 0.0;
        m_vertex[1].y = 0.5;
        m_vertex[1].z = 0.5;

        m_vertex[2].x = 0.0;
        m_vertex[2].y = -0.5;
        m_vertex[2].z = 0.5;

        m_vertex[3].x = 0.0;
        m_vertex[3].y = -0.5;
        m_vertex[3].z = -0.5;

        break;

    case E_TYPE_TEXTUREOBJ::HORIZON:
        m_vertex[0].x = -0.5;
        m_vertex[0].y = 0.0;
        m_vertex[0].z = 0.5;

        m_vertex[1].x = 0.5;
        m_vertex[1].y = 0.0;
        m_vertex[1].z = 0.5;

        m_vertex[2].x = -0.5;
        m_vertex[2].y = 0.0;
        m_vertex[2].z = -0.5;

        m_vertex[3].x = 0.5;
        m_vertex[3].y = 0.0;
        m_vertex[3].z = -0.5;
        break;

    case E_TYPE_TEXTUREOBJ::VERTICAL:
        m_vertex[0].x = -0.5;
        m_vertex[0].y = 0.5;
        m_vertex[0].z = 0.0;

        m_vertex[1].x = 0.5;
        m_vertex[1].y = 0.5;
        m_vertex[1].z = 0.0;

        m_vertex[2].x = -0.5;
        m_vertex[2].y = -0.5;
        m_vertex[2].z = 0.0;

        m_vertex[3].x = 0.5;
        m_vertex[3].y = -0.5;
        m_vertex[3].z = 0.0;
        break;
    }
    XMFLOAT4 color;

    std::tie(color.x, color.y, color.z, color.w) = m_gameObject->m_transform->m_color.GetValue();

    m_vertex[0].color = color;
    m_vertex[1].color = color;
    m_vertex[2].color = color;
    m_vertex[3].color = color;

    if (m_vertexbuffer == nullptr)
    {
        // 頂点バッファ作成（後で変更可能な）
        bool sts = CreateVertexBufferWrite(CDirectXGraphics::GetInstance().GetDXDevice(), //デバイス
                                           sizeof(tagVertex),                             //ストライド（1頂点当たりのバイト数）
                                           4,                                             //頂点数
                                           m_vertex,                                      //初期化データの先頭アドレス
                                           &m_vertexbuffer);                              //頂点バッファ
        if (!sts)
        {
            MessageBox(nullptr, "Com3DTexture SetVertex Error", "error", MB_OK);
        }
    }
    else
    {
        // 作成済みなら 内容を書き換える
        D3D11_MAPPED_SUBRESOURCE pData;

        HRESULT hr = CDirectXGraphics::GetInstance().GetImmediateContext()->Map(m_vertexbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
        if (SUCCEEDED(hr))
        {
            memcpy_s(pData.pData, pData.RowPitch, (void *)(m_vertex), sizeof(tagVertex) * 4);
            CDirectXGraphics::GetInstance().GetImmediateContext()->Unmap(m_vertexbuffer, 0);
        }
    }
}

//================================================================================================
//================================================================================================

void Com3DTexture::SetUV()
{
    m_vertex[0].tu = m_textureRate.x * (m_textureNum.x - 1);
    m_vertex[0].tv = m_textureRate.y * (m_textureNum.y - 1);

    m_vertex[1].tu = m_textureRate.x * (m_textureNum.x);
    m_vertex[1].tv = m_vertex[0].tv;

    m_vertex[2].tu = m_vertex[0].tu;
    m_vertex[2].tv = m_textureRate.y * (m_textureNum.y);

    m_vertex[3].tu = m_vertex[1].tu;
    m_vertex[3].tv = m_vertex[2].tv;

    SetVertex();
}