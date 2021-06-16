
#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include "../../../../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"
#include "../../../../WindowsSystem/Shader/Shader.h"
#include "CMesh.h"
#include "../../../../WindowsSystem/Dx11mathutil/Dx11mathutil.h"

using namespace std;
using namespace DirectX;

CMesh::CMesh(std::vector<tagAssimpVertex> _vertices, std::vector<unsigned int> _indices, std::vector<tagTexture> _textures, tagMaterial _mtrl)
{
    this->m_vertices = _vertices;
    this->m_indices = _indices;

    this->m_textures = _textures;
    this->m_mtrl = _mtrl;
    this->CreateBuffer();
    //		this->m_deformvertices.resize(vertices.size());			// GPU対応で不要

    std::vector<XMFLOAT3> tempv;

    // ボーンを登録
    for (int i = 0; i < _vertices.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (_vertices[i].m_BoneName[j] != "")
            {
                m_listBoneVertices[_vertices[i].m_BoneName[j]] = tempv;
            }
        }
    }

    // 頂点を登録
    for (int i = 0; i < _vertices.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // ボーン存在チェック
            auto itr = m_listBoneVertices.find(_vertices[i].m_BoneName[j].c_str());
            if (itr != m_listBoneVertices.end())
            {
                std::vector<XMFLOAT3> &vv = m_listBoneVertices[_vertices[i].m_BoneName[j].c_str()];
                vv.push_back(_vertices[i].m_Pos);
            }
        }
    }
}

//================================================================================================
//================================================================================================

void CMesh::Uninit()
{
    m_vertexBuffer->Release();
    m_indexBuffer->Release();
}

//================================================================================================
//================================================================================================

void CMesh::Draw()
{
    ID3D11DeviceContext *devcon = CDirectXGraphics::GetInstance().GetImmediateContext();
    unsigned int stride = sizeof(tagAssimpVertex);
    unsigned int offset = 0;

    // 頂点バッファをセット
    devcon->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    // インデックスバッファをセット
    devcon->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    // トポロジーをセット
    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // SRVをセット
    if (m_textures.size() >= 1)
    {
        devcon->PSSetShaderResources(0, 1, &m_textures[0].texture);
    }
    else
    {
        //白画像今ないのでコメントアウト
        //devcon->PSSetShaderResources(0, 1, &CDirectXGraphics::m_whitesrv);
    }

    // マテリアルを各シェーダーへセット
    devcon->PSSetConstantBuffers(3, 1, &m_cbmtrl);
    devcon->VSSetConstantBuffers(3, 1, &m_cbmtrl);

    // インデックスバッファを利用して描画
    devcon->DrawIndexed(static_cast<unsigned int>(m_indices.size()), 0, 0);
}

//================================================================================================
//================================================================================================

bool CMesh::CreateBuffer()
{
    ID3D11Device *device = CDirectXGraphics::GetInstance().GetDXDevice();

    // 頂点バッファ生成
    bool sts = CreateVertexBufferWrite(device,
                                       static_cast<unsigned int>(sizeof(tagAssimpVertex)),    // ストライド
                                       static_cast<unsigned int>(m_vertices.size()), // 頂点数
                                       m_vertices.data(),                            // 頂点データ
                                       &m_vertexBuffer);
    if (!sts)
    {
        return false;
    }

    // インデックスバッファ生成
    sts = CreateIndexBuffer(device,
                            static_cast<unsigned int>(m_indices.size()),
                            m_indices.data(),
                            &m_indexBuffer);
    if (!sts)
    {
        return false;
    }

    // マテリアル用コンスタントバッファ生成
    sts = CreateConstantBufferWrite(device,
                                    sizeof(tagConstantBufferMaterial),
                                    &m_cbmtrl);
    if (!sts)
    {
        return false;
    }

    ID3D11DeviceContext *devicecontext = CDirectXGraphics::GetInstance().GetImmediateContext();

    // 定数バッファ更新
    D3D11_MAPPED_SUBRESOURCE pData;

    tagConstantBufferMaterial cb;

    cb.AmbientMaterial.w = 1.0f;
    cb.AmbientMaterial.x = m_mtrl.m_Ambient.x;
    cb.AmbientMaterial.y = m_mtrl.m_Ambient.y;
    cb.AmbientMaterial.z = m_mtrl.m_Ambient.z;

    cb.DiffuseMaterial.w = 1.0f;
    cb.DiffuseMaterial.x = m_mtrl.m_Diffuse.x;
    cb.DiffuseMaterial.y = m_mtrl.m_Diffuse.y;
    cb.DiffuseMaterial.z = m_mtrl.m_Diffuse.z;

    cb.SpecularMaterial.w = 1.0f;
    cb.SpecularMaterial.x = m_mtrl.m_Specular.x;
    cb.SpecularMaterial.y = m_mtrl.m_Specular.y;
    cb.SpecularMaterial.z = m_mtrl.m_Specular.z;

    HRESULT hr = devicecontext->Map(m_cbmtrl, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
    if (SUCCEEDED(hr))
    {
        memcpy_s(pData.pData, pData.RowPitch, (void *)(&cb), sizeof(tagConstantBufferMaterial));
        devicecontext->Unmap(m_cbmtrl, 0);
    }

    return true;
}

void CMesh::UpdateVertexBuffer()
{

    HRESULT hr;

    ID3D11DeviceContext *devicecontext;

    devicecontext = CDirectXGraphics::GetInstance().GetImmediateContext();

    D3D11_MAPPED_SUBRESOURCE pData;

    // 定数バッファ更新
    hr = devicecontext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
    if (SUCCEEDED(hr))
    {
        //			memcpy_s(pData.pData, pData.RowPitch, (void*)m_deformvertices.data(), sizeof(aiVertex)*m_deformvertices.size());
        memcpy_s(pData.pData, pData.RowPitch, (void *)m_vertices.data(), sizeof(tagAssimpVertex) * m_vertices.size());
        devicecontext->Unmap(m_vertexBuffer, 0);
    }
}