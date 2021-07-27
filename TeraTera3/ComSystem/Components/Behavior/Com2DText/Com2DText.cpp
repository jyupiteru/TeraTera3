/**
 * @file Com2DText.cpp
 * @author jupiter ()
 * @brief Com2DTextの実装が書かれたcpp
 */
#include "Com2DText.h"
#include "../../../../Setup.h"
#include "../../../../WindowsSystem/DX11Settransform.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../ImGuiSystem/ImGuiHelperFunctions.h"
#include "../ComTransform/ComTransform.h"

std::unordered_map<std::string, ID3D11ShaderResourceView *> Com2DText::m_pListSRV;

std::unordered_map<std::string, ID3D11Resource *> Com2DText::m_pListFontTexture;

int Com2DText::m_classCount = 0;

Com2DText::tagConstantBufferViewPort Com2DText::m_screenData;

void Com2DText::Init()
{
    //表示レイヤーの変更
    m_gameObject->m_drawLayer.SetValue(6);

    m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT2D);

    m_flagSetRate.SetValue(true);

    m_listVertex.resize(MAXSTRINGSIZE * 4);

    m_listIndex.resize(MAXSTRINGSIZE * 4 * 2);

    CreateVertexIndexData();

    ID3D11Device *device = CDirectXGraphics::GetInstance().GetDXDevice();

    // 頂点バッファ生成
    bool sts = CreateVertexBufferWrite(device, sizeof(tagVertex), MAXSTRINGSIZE * 4, m_listVertex.data(), &m_vertexBuffer);
    if (!sts)
    {
        MessageBox(nullptr, "CreateVerteBuffer error", "error", MB_OK);
    }

    // インデックスバッファ生成
    sts = CreateIndexBufferWrite(device, MAXSTRINGSIZE * 4 * 2, m_listIndex.data(), &m_idxBuffer);
    if (!sts)
    {
        MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
    }

    // 定数バッファ作成
    sts = CreateConstantBuffer(
        device,
        sizeof(tagConstantBufferViewPort),
        &m_screenBuffer);
    if (!sts)
    {
        MessageBox(NULL, "screenbuffer error", "Error", MB_OK);
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

    unsigned int numelements = ARRAYSIZE(layout);

    LoadFontTexture("FontImage.dds");

    ClearString();

    m_classCount++;
}

//================================================================================================
//================================================================================================

void Com2DText::Uninit()
{
    if (m_screenBuffer)
    {
        m_screenBuffer->Release();
        m_screenBuffer = nullptr;
    }
    m_classCount--;
    if (m_classCount <= 0)
    {
        if (m_pListSRV.empty() == false)
        {
            m_pListSRV.clear();
        }
        if (m_pListFontTexture.empty() == false)
        {
            m_pListFontTexture.clear();
        }
    }

    m_listVertex.clear();

    m_listIndex.clear();
}

//================================================================================================
//================================================================================================

void Com2DText::Ready()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

    unsigned int numelements = ARRAYSIZE(layout);
    m_pShader = m_gameObject->GetComponent<ComShader>();
    if (m_pShader == nullptr)
    {
        m_pShader = m_gameObject->AddComponent<ComShader>();
    }
    m_pShader->LoadVertexShader("VSUItex.fx", layout, numelements, true);
    m_pShader->LoadPixelShader("PSTexWithColor.fx", true);
}

//================================================================================================
//================================================================================================

void Com2DText::Update()
{
    ClearString();

    CreateVertexIndexData();

    UpdateBuffer();
}

//================================================================================================
//================================================================================================

void Com2DText::Draw()
{
    DirectX::XMFLOAT3 trans = {0, 0, 0};
    DirectX::XMFLOAT4X4 mtx;

    DX11MtxTranslation(trans, mtx);
    DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

    m_screenData.viewPortHeight.x = SCREEN_HEIGHT;
    m_screenData.viewPortWidth.x = SCREEN_WIDTH;

    // デバイスコンテキストを取得
    ID3D11DeviceContext *devcontext;
    //	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
    devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();

    //シェーダにスクリーンサイズをセット
    devcontext->UpdateSubresource(m_screenBuffer, 0, nullptr, &m_screenData, 0, 0);
    devcontext->VSSetConstantBuffers(5, 1, &m_screenBuffer);

    //これをONにすると強制的にほかのものに上書きできる(一番手前になる)
    CDirectXGraphics::GetInstance().TurnOffZbuffer();

    unsigned int stride = sizeof(tagVertex); // ストライドをセット（１頂点当たりのバイト数）
    unsigned offset = 0;                     // オフセット値をセット

    // 頂点バッファをデバイスコンテキストへセット
    devcontext->IASetVertexBuffers(
        0,               // スタートスロット
        1,               // 頂点バッファ個数
        &m_vertexBuffer, // 頂点バッファの先頭アドレス
        &stride,         // ストライド
        &offset);        // オフセット

    // インデックスバッファをデバイスコンテキストへセット
    devcontext->IASetIndexBuffer(
        m_idxBuffer,          // インデックスバッファ
        DXGI_FORMAT_R32_UINT, // フォーマット
        0);                   // オフセット

    // トポロジーをセット
    devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pShader->SetPixelShader();
    m_pShader->SetVertexShader();

    ID3D11ShaderResourceView *srv = m_pListSRV[m_keyFontTexture];

    // SRVをセット
    // PSにSRVをセット
    devcontext->PSSetShaderResources(
        0,     //t0レジスタ
        1,     //個数
        &srv); //SRV

    // ドローコール発行
    devcontext->DrawIndexed(
        static_cast<unsigned int>(m_listIndex.size()), // インデックス数
        0,                                             // 開始インデックス
        0);                                            // 基準頂点インデックス

    CDirectXGraphics::GetInstance().TurnOnZBuffer();
}

//================================================================================================
//================================================================================================

void Com2DText::ImGuiDraw(unsigned int windowid)
{
    ImGui::BulletText("Font name : %s", m_keyFontTexture.c_str());
    ImGui::BulletText("Draw Text : %s", m_text.c_str());

    if (m_flagSetRate.GetValue())
    {
        ImGui::BulletText("FlagSetRate : true");
    }
    else
    {
        ImGui::BulletText("FlagSetRate : false");
    }
    ImGui::SameLine();
    HelpMarker((const char *)u8"表示する際に縦横の文字の比率を合わせるか");
}

//================================================================================================
//================================================================================================

void Com2DText::LoadFontTexture(std::string_view fontname)
{

    // 定数バッファ生成
    bool sts = CreateConstantBufferWrite(CDirectXGraphics::GetInstance().GetDXDevice(), sizeof(tagConstantBufferViewPort), &m_cbuffer);
    if (!sts)
    {
        MessageBox(nullptr, TEXT("CreateConstantBufferWrite error"), TEXT("error"), MB_OK);
    }

    m_keyFontTexture = fontname;

    std::string folder = "Assets/Font/";

    folder += fontname;

    if (!m_pListFontTexture.contains(m_keyFontTexture))
    {
        ID3D11ShaderResourceView *srv = nullptr;
        ID3D11Resource *texres = nullptr;

        // SRV生成
        sts = CreateSRVfromFile(folder.c_str(), //画像ファイル名
                                CDirectXGraphics::GetInstance().GetDXDevice(),
                                CDirectXGraphics::GetInstance().GetImmediateContext(),
                                &texres,
                                &srv);
        if (!sts)
        {
            MessageBox(nullptr, TEXT("CreateSRVfromFile error"), TEXT("error"), MB_OK);
        }

        m_pListSRV[m_keyFontTexture] = srv;
        m_pListFontTexture[m_keyFontTexture] = texres;
    }
}

//================================================================================================
//================================================================================================

void Com2DText::ClearString(void)
{
    m_listVertex.clear();
    m_listVertex.resize(MAXSTRINGSIZE * 4);

    m_listIndex.clear();
    m_listIndex.resize(MAXSTRINGSIZE * 4 * 2);
}

//================================================================================================
//================================================================================================

void Com2DText::UpdateBuffer()
{
    D3D11_MAPPED_SUBRESOURCE pData;
    ID3D11DeviceContext *devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();

    //頂点バッファの更新
    HRESULT hr = devcontext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
    if (SUCCEEDED(hr))
    {
        memcpy_s(pData.pData, pData.RowPitch, (void *)(m_listVertex.data()), sizeof(tagVertex) * m_listVertex.size());
        devcontext->Unmap(m_vertexBuffer, 0);
    }

    //インデックスの更新
    hr = devcontext->Map(m_idxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
    if (SUCCEEDED(hr))
    {
        memcpy_s(pData.pData, pData.RowPitch, (void *)(m_listIndex.data()), sizeof(unsigned int) * m_listIndex.size());
        devcontext->Unmap(m_idxBuffer, 0);
    }
}

//================================================================================================
//================================================================================================

void Com2DText::CreateVertexIndexData()
{

    auto [size_x, size_y, size_z] = m_gameObject->m_transform->m_size.GetValue();

    auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue();

    const auto centerpos_x = SCREEN_WIDTH / 2;
    const auto centerpos_y = SCREEN_HEIGHT / 2;

    //文字列の長さ
    int stringsize = static_cast<int>(m_text.size());

    //1文字当たりの大きさを割り出し
    auto perhalfsize_x = size_x / stringsize;
    perhalfsize_x /= 2.0f;
    auto perhalfsize_y = size_y / 2.0f;

    if (perhalfsize_x * 2 < perhalfsize_y)
    { //１文字あたりのxの大きさがyの大きさを超えない場合(文字列の表示時の大きさが縦に崩れるのを防ぐ)
        perhalfsize_y = perhalfsize_x * 2;
    }
    else
    { //yの大きさを超える場合（横幅の縮小）
        //todo 今頭働かないのでここの記述追加
        //size_x = size_y / 2;
    }

    //全体の中心座標（スクリーンのもの）
    auto startpos_x = centerpos_x + pos_x * 2;

    //文字列の一番初めの中心に移動
    startpos_x -= size_x / 2 - perhalfsize_x;

    pos_y = centerpos_y - pos_y * 2;

    int vcnt = 0;    // 頂点数
    int icnt = 0;    // インデックス数
    int quadnum = 0; // 四角形の数

    auto [color_r, color_g, color_b, color_z] = m_gameObject->m_transform->m_color.GetValue();

    DirectX::XMFLOAT2 textuv[4];

    for (int idx = 0; idx < stringsize; idx++)
    {
        vcnt = idx * 4;
        // 文字コードからＡＳＣＩＩフォントのＵＶ座標を求める
        GetFontUV(m_text[idx], textuv);

        // 頂点座標作成
        //--
        m_listVertex[vcnt].Color = {color_r, color_g, color_b, color_z};

        m_listVertex[vcnt].Pos.x = static_cast<float>(startpos_x - perhalfsize_x);
        m_listVertex[vcnt].Pos.y = static_cast<float>(pos_y - perhalfsize_y);
        m_listVertex[vcnt].Pos.z = 0.0f;

        m_listVertex[vcnt].Tex.x = textuv[0].x;
        m_listVertex[vcnt].Tex.y = textuv[0].y;
        vcnt++;

        //+-
        m_listVertex[vcnt].Color = {color_r, color_g, color_b, color_z};
        m_listVertex[vcnt].Pos.x = static_cast<float>(startpos_x + perhalfsize_x);
        m_listVertex[vcnt].Pos.y = m_listVertex[vcnt - 1].Pos.y;
        m_listVertex[vcnt].Pos.z = 0.0f;

        m_listVertex[vcnt].Tex.x = textuv[1].x;
        m_listVertex[vcnt].Tex.y = textuv[1].y;
        vcnt++;

        //-+
        m_listVertex[vcnt].Color = {color_r, color_g, color_b, color_z};

        m_listVertex[vcnt].Pos.x = m_listVertex[vcnt - 2].Pos.x;
        m_listVertex[vcnt].Pos.y = static_cast<float>(pos_y + perhalfsize_y);
        m_listVertex[vcnt].Pos.z = 0.0f;

        m_listVertex[vcnt].Tex.x = textuv[2].x;
        m_listVertex[vcnt].Tex.y = textuv[2].y;
        vcnt++;

        //++
        m_listVertex[vcnt].Color = {color_r, color_g, color_b, color_z};
        m_listVertex[vcnt].Pos.x = m_listVertex[vcnt - 2].Pos.x;
        m_listVertex[vcnt].Pos.y = m_listVertex[vcnt - 1].Pos.y;
        m_listVertex[vcnt].Pos.z = 0.0f;

        m_listVertex[vcnt].Tex.x = textuv[3].x;
        m_listVertex[vcnt].Tex.y = textuv[3].y;
        vcnt++;

        // インデックスデータ作成
        m_listIndex[icnt].idx[0] = quadnum * 4;
        m_listIndex[icnt].idx[1] = quadnum * 4 + 1;
        m_listIndex[icnt].idx[2] = quadnum * 4 + 2;
        icnt++;

        m_listIndex[icnt].idx[0] = quadnum * 4 + 1;
        m_listIndex[icnt].idx[1] = quadnum * 4 + 3;
        m_listIndex[icnt].idx[2] = quadnum * 4 + 2;

        icnt++;
        quadnum++;

        //開始位置をずらす（各文字の位置が違うため）
        startpos_x += perhalfsize_x * 2.0f;
    }
}

//================================================================================================
//================================================================================================

void Com2DText::GetFontUV(char chara, DirectX::XMFLOAT2 *fontuv)
{
    //todo 現状法則性がわからず専用になっているので修正する
    //縦横の文字列数
    int max_y = 1;
    int max_x = 95;

    //縦横の1個当たりの比率
    float rate_x = 1.0f / max_x;
    float rate_y = 1.0f / max_y;

    int codex;
    int codey;

    codex = chara - 32;
    codey = 2;

    codey -= 2;

    float tu, tv;

    tu = codex * rate_x;
    tv = codey * rate_y;

    fontuv[0].x = tu;
    fontuv[0].y = tv;

    fontuv[1].x = tu + rate_x;
    fontuv[1].y = tv;

    fontuv[2].x = tu;
    fontuv[2].y = tv + rate_y;

    fontuv[3].x = tu + rate_x;
    fontuv[3].y = tv + rate_y;
}
