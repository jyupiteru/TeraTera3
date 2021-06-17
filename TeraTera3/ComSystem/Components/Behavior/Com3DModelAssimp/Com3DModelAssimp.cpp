/**
 * @file Com3DModelAssimp.cpp
 * @author jupiter
 * @brief Com3DModelAssimpの実装の一部が書かれたcpp
 * @version 1.0
 * @date 2020-08-08
 */

#define NOMINMAX

#include "Com3DModelAssimp.h"
#include "../../../../System/CMatrix/CMatrix.h"
#include "../../../Core/GameObject.h"
#include "../ComTransform/ComTransform.h"
#include "../../../../WindowsSystem/DX11Settransform.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"

CListResource *Com3DModelAssimp::m_pListModel;

int Com3DModelAssimp::m_classCount = 0;

void Com3DModelAssimp::Init()
{
    m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT3D);
    m_classCount++;

    //staticな変数として存在しているか
    if (m_pListModel == nullptr)
    {
        //コンテナとリストへのアクセス権の登録
        CContainer::GetInstance().MakePermissionToContainer(this);
        CContainer::GetInstance().MakePermissionToList(this, "assimpmodeldata");
        //リストの取得
        m_pListModel = CContainer::GetInstance().GetListResource(this, "assimpmodeldata");
        m_pListModel->SetResourceType<tagAssimpModelData>();
    }

    DX11MtxIdentity(m_modelMatrix); //単位行列化 つまり初期化

    m_pShader = m_gameObject->GetComponent<ComShader>();
    if (m_pShader == nullptr)
    {
        m_pShader = m_gameObject->AddComponent<ComShader>();
    }

    // 頂点データの定義（アニメーション対応）
    D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
    unsigned int numElements = ARRAYSIZE(layout);

    m_pShader->LoadVertexShader("VSAssimpModel.fx", layout, numElements, true);
    m_pShader->LoadPixelShader("pslambert.fx", true);
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::Uninit()
{
    if (m_pNowModelData != nullptr)
    {
        m_pNowModelData = nullptr;
    }

    m_classCount--;
    if (m_classCount <= 0)
    {
        m_pListModel = nullptr;
    }
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::Ready()
{
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::Update()
{
    CMatrix *mtx = new CMatrix();

    mtx->SetMatrix(m_gameObject->m_transform->GetNoScalingMatrix());

    auto [size_x, size_y, size_z] = m_gameObject->m_transform->m_size.GetValue();

    { //中心座標の設定
        auto [pos_x, pos_y, pos_z] = m_gameObject->m_transform->m_worldPosition.GetValue();
        mtx->SetPos(pos_x, pos_y, pos_z);
    }

    { //座標の変更設定（モデルのワールド中心座標とローカル中心座標の差を埋める）

        //モデルのローカル中心座標(大きさ調整後)
        auto [localpos_x, localpos_y, localpos_z] = m_localPosition.GetValue();

        //モデルの0,0,0とワールドの0,0,0（疑似的）は違うのでモデルのほうに直す
        mtx->SetVec(localpos_x * size_x,
                    localpos_y * size_y,
                    localpos_z * size_z);
    }

    { //大きさの設定
        auto rate = m_gameObject->m_transform->m_sizeRateSize.GetValue();

        mtx->SetSize(size_x * rate, size_y * rate, size_z * rate);
    }

    mtx->Update();

    m_modelMatrix = mtx->GetMatrix();

    delete mtx;
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::Draw()
{
    if (m_pNowModelData != nullptr)
    {
        auto [color_r, color_g, color_b, color_a] = m_gameObject->m_transform->m_color.GetValue();

        m_pShader->SetPixelShader();
        m_pShader->SetVertexShader();

        // モデル描画
        m_pNowModelData->modeldata.Draw((DirectX::XMFLOAT4X4 &)m_modelMatrix, m_animationData, DirectX::XMFLOAT4(color_r, color_g, color_b, color_a));
    }
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::ImGuiDraw(unsigned int windowid)
{
    ImGui::BulletText("ModelName : %s", m_keyModel.c_str());
    ImGui::Text("Here has nodisplay things");
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::LoadModelData(std::string modelname, std::string texturefolder)
{
    std::string folder = "Assets/Models/";

    m_keyModel = modelname;

    modelname = folder + modelname;
    texturefolder = folder + texturefolder;

    //モデルすでに読み込まれているかどうか
    if (!m_pListModel->CheckResourceData(m_keyModel))
    {
        std::shared_ptr<tagAssimpModelData> date = std::make_shared<tagAssimpModelData>();
        bool sts = date->modeldata.Load(texturefolder, modelname);
        if (sts)
        {
            m_pListModel->SetResourceData(m_keyModel, date);
            SetModelRate();
        }
        else
        {
            MessageBox(nullptr, TEXT(modelname.c_str()), TEXT("error"), MB_OK);
            return;
        }
    }

    //現在使用するモデルのデータを読み込む
    m_pNowModelData = m_pListModel->GetResourceData<tagAssimpModelData>(m_keyModel);

    m_animationData.m_listBone = m_pNowModelData->modeldata.m_Bone;

    auto rate = 1.0f / m_pNowModelData->sizeforrate;

    m_gameObject->m_transform->m_sizeRateSize.SetValue(rate);

    m_localPosition.SetValue((0.0f - m_pNowModelData->center_x) * rate,
                             (0.0f - m_pNowModelData->center_y) * rate,
                             (0.0f - m_pNowModelData->center_z) * rate);

    //モデルの統一前の大きさを格納
    m_gameObject->m_transform->m_offsetSize.SetValue(m_pNowModelData->max_x - m_pNowModelData->min_x,
                                                     m_pNowModelData->max_y - m_pNowModelData->min_y,
                                                     m_pNowModelData->max_z - m_pNowModelData->min_z);
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::SetModelRate()
{
    DirectX::XMFLOAT3 max = {0, 0, 0}, min = {0, 0, 0};

    auto resourcedata = m_pListModel->GetResourceData<tagAssimpModelData>(m_keyModel);
    auto meshs = &resourcedata->modeldata.GetMeshes();

    //最も大きいところにあるメッシュを検索
    //髪とか何かほかのところにあるとおそらく失敗する
    for (auto &itr : *meshs)
    {
        for (auto &itr2 : itr.m_vertices)
        {
            CheckVolume(itr2.m_Pos.x, max.x, min.x);
            CheckVolume(itr2.m_Pos.y, max.y, min.y);
            CheckVolume(itr2.m_Pos.z, max.z, min.z);
        }
    }

    {
        resourcedata->max_x = max.x;
        resourcedata->max_y = max.y;
        resourcedata->max_z = max.z;

        resourcedata->min_x = min.x;
        resourcedata->min_y = min.y;
        resourcedata->min_z = min.z;
    }

    auto size_x = max.x - min.x;
    auto size_y = max.y - min.y;
    auto size_z = max.z - min.z;

    //モデルの中心座標をセット
    resourcedata->center_x = max.x - size_x / 2;
    resourcedata->center_y = max.y - size_y / 2;
    resourcedata->center_z = max.z - size_z / 2;

    //最も大きいものを大きさ統一の際の基準にするためにセット
    float size = size_x;
    if (size < size_y)
    {
        size = size_y;
    }
    if (size < size_z)
    {
        size = size_z;
    }
    resourcedata->sizeforrate = size;
}

//================================================================================================
//================================================================================================

void Com3DModelAssimp::CheckVolume(float num, float &nowmax, float &nowmin)
{
    if (num > nowmax)
    {
        nowmax = num;
    }
    else if (num < nowmin)
    {
        nowmin = num;
    }
}