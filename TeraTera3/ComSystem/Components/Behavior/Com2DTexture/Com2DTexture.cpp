/**
 * @file Com2DTexture.cpp
 * @author jupiter
 * @brief Com2DTextureクラスの処理が書かれたcpp
 */
#include "Com2DTexture.h"

#include "../ComTransform/ComTransform.h"
#include "../../../../System/CMatrix/CMatrix.h"
#include "../../../../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"

ConstantBufferViewPort Com2DTexture::m_screenData;

void Com2DTexture::Init()
{
    //表示レイヤーの変更
    m_gameObject->m_drawLayer.SetValue(5);

    m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT2D);
    this->LoadTexture("System/white.png");

    ID3D11Device *device = CDirectXGraphics::GetInstance().GetDXDevice();

    // 定数バッファ作成
    bool sts = CreateConstantBuffer(
        device,
        sizeof(ConstantBufferViewPort),
        &m_screenBuffer);
    if (!sts)
    {
        MessageBox(NULL, "screenbuffer error", "Error", MB_OK);
    }
}

//================================================================================================
//================================================================================================

void Com2DTexture::Uninit()
{
    Com3DTexture::Uninit();
}

//================================================================================================
//================================================================================================

void Com2DTexture::Ready()
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
    m_pShader->LoadVertexShader("VSUITex.fx", layout, numElements, true);
    m_pShader->LoadPixelShader("PSTexWithColor.fx", true);
}

//================================================================================================
//================================================================================================

void Com2DTexture::Draw()
{
    DirectX::XMFLOAT3 trans = {0, 0, 0};
    DirectX::XMFLOAT4X4 mtx;

    DX11MtxTranslation(trans, mtx);
    m_gameObject->m_transform->SetMatrix(mtx);

    m_screenData.viewPortHeight.x = SCREEN_HEIGHT;
    m_screenData.viewPortWidth.x = SCREEN_WIDTH;

    //シェーダにスクリーンサイズをセット
    CDirectXGraphics::GetInstance().GetImmediateContext()->UpdateSubresource(m_screenBuffer, 0, nullptr, &m_screenData, 0, 0);
    CDirectXGraphics::GetInstance().GetImmediateContext()->VSSetConstantBuffers(5, 1, &m_screenBuffer);

    //これをONにすると強制的にほかのものに上書きできる(一番手前になる)
    CDirectXGraphics::GetInstance().TurnOffZbuffer();

    Com3DTexture::Draw();

    CDirectXGraphics::GetInstance().TurnOnZBuffer();
}

//================================================================================================
//================================================================================================

void Com2DTexture::ImGuiDraw(unsigned int windowid)
{
    Com3DTexture::ImGuiDraw(windowid);
}

//================================================================================================
//================================================================================================

void Com2DTexture::SetVertex()
{
    float size_x, size_y;

    // m_vertex[0].x = m_gameObject->GetComponent<ComTransform>()->m_worldPosition.GetValue().x;
    // m_vertex[0].y = m_gameObject->GetComponent<ComTransform>()->m_worldPosition.GetValue().y;

    std::tie(size_x, size_y, std::ignore) = m_gameObject->m_transform->m_size.GetValue();

    auto [tmp_pos_x, tmp_pos_y, a] = m_gameObject->m_transform->m_worldPosition.GetValue();

    //--
    //画面の中心に座標を設定
    auto pos_x = SCREEN_WIDTH / 2;
    auto pos_y = SCREEN_HEIGHT / 2;

    //オブジェクトの座標まで移動&y軸の上下反転
    pos_x += static_cast<int>(tmp_pos_x * 2);
    pos_y -= static_cast<int>(tmp_pos_y * 2);

    //todo ここをどうにかすればUIの回転簡単に適応できるのでは？
    m_vertex[0].x = pos_x - size_x;
    m_vertex[0].y = pos_y - size_y;

    m_vertex[1].x = pos_x + size_x;
    m_vertex[1].y = m_vertex[0].y;

    //-+
    m_vertex[2].x = m_vertex[0].x;
    m_vertex[2].y = pos_y + size_y;

    //++
    m_vertex[3].x = m_vertex[1].x;
    m_vertex[3].y = m_vertex[2].y;

    int i = 0;
    while (i < 4)
    {
        std::tie(m_vertex[i].color.x,
                 m_vertex[i].color.y,
                 m_vertex[i].color.z,
                 m_vertex[i].color.w) = m_gameObject->m_transform->m_color.GetValue();
        m_vertex[i].z = 0.0f;
        i++;
    }

    if (m_vertexbuffer == nullptr)
    {
        // 頂点バッファ作成（後で変更可能なもの）
        bool sts = CreateVertexBufferWrite(CDirectXGraphics::GetInstance().GetDXDevice(),   //デバイス
                                           sizeof(tagVertex), //ストライド（1頂点当たりのバイト数）
                                           4,                 //頂点数
                                           m_vertex,          //初期化データの先頭アドレス
                                           &m_vertexbuffer);  //頂点バッファ
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