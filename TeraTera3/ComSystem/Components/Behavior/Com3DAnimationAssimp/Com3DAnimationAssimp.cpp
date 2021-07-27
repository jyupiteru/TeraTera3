/**
 * @file Com3DAnimationAssimp.cpp
 * @author jupiter ()
 * @brief Com3DAnimationAssimpの実装が書かれたcpp
 */

#include "Com3DAnimationAssimp.h"
#include "../Com3DModelAssimp/Com3DModelAssimp.h"
#include "../ComShader/ComShader.h"
#include "../ComTransform/ComTransform.h"
#include "../../../../Setup.h"
#include "../../../../Timer/CTimer.h"
#include "../../../../../ThirdParty/ImGui/imgui.h"
#include "../../../../ImGuiSystem/ImGuiHelperFunctions.h"

/**
 * @brief アニメーションデータをモデルごとに格納、管理する変数
 * @n モデルのキー、アニメーションデータ
 */
CListResource *Com3DAnimationAssimp::m_listAssimpAnimation = nullptr;

unsigned int Com3DAnimationAssimp::m_classCount = 0;

void Com3DAnimationAssimp::Init()
{
    m_classCount++;

    if (m_listAssimpAnimation == nullptr)
    {
        //コンテナとリストへのアクセス権の作成
        CContainer::GetInstance().MakePermissionToContainer(this);
        CContainer::GetInstance().MakePermissionToList(this, "assimpanimationdata");

        //リストの取得とリストに登録するタイプを取得
        m_listAssimpAnimation = CContainer::GetInstance().GetListResource(this, "assimpanimationdata");
        m_listAssimpAnimation->SetResourceType<taglistAssimpAnimation>();
    }

    m_pCom3DModel = m_gameObject->GetComponent<Com3DModelAssimp>();
    if (m_pCom3DModel == nullptr)
    { //存在しないなら追加している意味はないので
        this->m_enable.SetValue(false);
    }
    else
    {
        //すでにアニメーションを読み込んだことがあるなら登録されているのでアニメーションリストを取得 これでnullptrならまだ未登録
        m_pNowAnimationData = m_listAssimpAnimation->GetResourceData<taglistAssimpAnimation>(m_pCom3DModel->m_keyModel);
    }
}

//================================================================================================
//================================================================================================

void Com3DAnimationAssimp::Uninit()
{
    m_pNowAnimationData = nullptr;
    m_classCount--;
    if (m_classCount <= 0)
    {
        m_listAssimpAnimation = nullptr;
    }
}

//================================================================================================
//================================================================================================

void Com3DAnimationAssimp::Ready()
{

    // 頂点データの定義（アニメーション対応）
    D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
    unsigned int numelements = ARRAYSIZE(layout);

    ComShader *shader = this->m_gameObject->GetComponent<ComShader>();
    shader->LoadVertexShader("VSOneskin.fx", layout, numelements, true);
}

//================================================================================================
//================================================================================================

void Com3DAnimationAssimp::Update()
{
    //アニメーションを読み込んだことがある
    if (m_pNowAnimationData != nullptr)
    {
        const aiScene *scene = m_pNowAnimationData->m_listAnimation[m_nowAnimation.first]->GetScene();
        tagAssimpModelData *data = m_pCom3DModel->m_pNowModelData;

        data->modeldata.UpdateAnimation(m_gameObject->m_transform->GetMatrix(), scene,
                                        m_nowAnimation.second, m_pCom3DModel->m_animationData);

        //アニメーションの速度を調整する
        ChangeAnimationFrame();
    }
}

//================================================================================================
//================================================================================================

void Com3DAnimationAssimp::ImGuiDraw(unsigned int windowid)
{
    ImGui::BulletText("NowAnimationName : %s", m_nowAnimation.first);
    ImGui::BulletText("NowAnimationId : %d", m_nowAnimation.second);

    ImGui::BulletText("AnimationGroupTotal : %d", m_pNowAnimationData->m_listAnimationName.size());
    ImGui::BulletText("NowAnimationId : %d", m_nowAnimation.second);

    ImGui::Text("Here has nodisplay things");

    //todo この下のもの追加すること
}

//================================================================================================
//================================================================================================

int Com3DAnimationAssimp::LoadAnimation(std::string_view groupname, std::string_view animationpass)
{
    bool flag = false;

    //アニメーションを読み込んだことがあるモデルか
    if (m_listAssimpAnimation->CheckResourceData(m_pCom3DModel->m_keyModel))
    { //リストの中にそれが存在している確証がないので2重に

        auto data = m_listAssimpAnimation->GetResourceData<taglistAssimpAnimation>(m_pCom3DModel->m_keyModel);

        //このアニメーションは読み込んだことがないか
        if (!data->m_listAnimation.contains(groupname.data()))
        {
            flag = true;
        }
    }
    else
    {
        flag = true;
    }

    //このアニメーションを読み込んだことがない
    if (flag)
    {
        std::shared_ptr<CAssimpAnimationData> animdata;
        animdata = std::make_shared<CAssimpAnimationData>();

        //フォルダを移動
        std::string str = "Assets/Models/";
        str += animationpass.data();

        //アニメーションを読み込み
        bool sts = animdata->Init(str);

        //読み込みは成功しているか
        if (sts)
        { //読み込み成功
            m_pNowAnimationData = m_listAssimpAnimation->GetResourceData<taglistAssimpAnimation>(m_pCom3DModel->m_keyModel);

            //このモデルのアニメーションリストは存在しているかどうか
            if (m_pNowAnimationData == nullptr)
            {
                std::shared_ptr<taglistAssimpAnimation> animationlist;
                animationlist = std::make_shared<taglistAssimpAnimation>();

                m_listAssimpAnimation->SetResourceData(m_pCom3DModel->m_keyModel, animationlist);
            }
            m_pNowAnimationData = m_listAssimpAnimation->GetResourceData<taglistAssimpAnimation>(m_pCom3DModel->m_keyModel);

            m_pNowAnimationData->m_listAnimation[groupname.data()] = animdata;
        }
        else
        {

            MessageBox(nullptr, "Load Animation error", "error", MB_OK);
            return -1;
        }
    }

    m_pNowAnimationData = m_listAssimpAnimation->GetResourceData<taglistAssimpAnimation>(m_pCom3DModel->m_keyModel);

    m_nowAnimation.first = groupname.data();
    m_nowAnimation.second = 0;

    //このアニメーショングループに存在しているアニメーションの総数
    return GetAnimationInGroupTotal();
}

//================================================================================================
//================================================================================================

bool Com3DAnimationAssimp::SetAnimationName(std::string_view groupname, unsigned int num, std::string_view animationname)
{
    //モデルを一度でも読み込んだことがあるか
    if (m_pNowAnimationData != nullptr)
    {
        //このグループは読み込んだことがあるか
        if (m_pNowAnimationData->m_listAnimation.contains(groupname.data()))
        {
            m_pNowAnimationData->m_listAnimationName[animationname.data()] = std::make_pair(groupname.data(), num);
            return true;
        }
    }
    return false;
}

//================================================================================================
//================================================================================================

int Com3DAnimationAssimp::GetAnimationGroupTotal()
{
    //ComModelAssimpは追加されている
    if (m_pCom3DModel != nullptr)
    {
        return static_cast<int>(m_pNowAnimationData->m_listAnimation.size());
    }
    return -1;
}

//================================================================================================
//================================================================================================

int Com3DAnimationAssimp::GetAnimationInGroupTotal()
{
    //ComModelAssimpは追加されているandアニメーションは読み込んでいる ここでアニメーショングループが存在するかは判別しなくていい(存在しないと変えられないようにしているので)
    if (m_pCom3DModel != nullptr && m_pNowAnimationData != nullptr)
    {
        return m_pNowAnimationData->m_listAnimation[m_nowAnimation.first]->GetScene()->mNumAnimations;
    }

    return -1;
}

//================================================================================================
//================================================================================================

int Com3DAnimationAssimp::GetAnimationInGroupTotal(std::string groupname)
{
    //ComModelAssimpは追加されているandアニメーションは読み込んでいる
    if (m_pCom3DModel != nullptr && m_pNowAnimationData != nullptr)
    {
        return m_pNowAnimationData->m_listAnimation[groupname.data()]->GetScene()->mNumAnimations;
    }
    return -1;
}

//================================================================================================
//================================================================================================

bool Com3DAnimationAssimp::IsFinish()
{
    //アニメーションは読み込んだことがあるか
    if (m_pCom3DModel != nullptr)
    {
        //最初のフレームでないか(0で乗算できないのでここでif)
        if (m_pCom3DModel->m_animationData.m_frame != 0)
        {
            //現在のアニメーションフレームが最後に到達したか
            if (m_pCom3DModel->m_animationData.m_frame % m_pCom3DModel->m_animationData.m_maxflame == 0)
            {
                return true;
            }
        }
    }
    return false;
}

//================================================================================================
//================================================================================================

bool Com3DAnimationAssimp::ChangeAnimation(std::string animationname)
{
    //アニメーションは読み込んだことがあるか
    if (m_pNowAnimationData != nullptr)
    {
        //そのアニメーションは登録されているか
        if (m_pNowAnimationData->m_listAnimationName.contains(animationname.data()))
        {
            ResetFrame();

            //次のアニメーションを取得
            auto anim = m_pNowAnimationData->m_listAnimationName[animationname.data()];

            //現在のアニメーションとは別か?(グループ名or番号はどちらかだけでも違うか)
            if (anim.first != m_nowAnimation.first || anim.second != m_nowAnimation.second)
            {
                m_nowAnimation = anim;

                const aiScene *scene = m_pNowAnimationData->m_listAnimation[m_nowAnimation.first]->GetScene();
                tagAssimpModelData *data = m_pCom3DModel->m_pNowModelData;

                //最大フレーム数等を更新
                data->modeldata.ChangeAnimation(scene,
                                                m_nowAnimation.second,
                                                m_pCom3DModel->m_animationData);

                return true;
            }
        }
    }
    return false;
}

//================================================================================================
//================================================================================================

bool Com3DAnimationAssimp::ChangeAnimation(std::string groupname, int num)
{
    //アニメーションは読み込んだことがあるか
    if (m_pNowAnimationData != nullptr)
    {
        //そのアニメーショングループは登録されたことがあるか
        if (m_pNowAnimationData->m_listAnimation.contains(groupname.data()))
        {
            //そのグループに登録されている最大数をこえていないか
            if (GetAnimationInGroupTotal(groupname.data()) > num)
            {
                m_nowAnimation = {groupname.data(), num};

                m_pCom3DModel->m_animationData.m_frame = 0;
                const aiScene *scene = m_pNowAnimationData->m_listAnimation[m_nowAnimation.first]->GetScene();
                tagAssimpModelData *data = m_pCom3DModel->m_pNowModelData;

                //最大フレーム数等を更新
                data->modeldata.ChangeAnimation(scene,
                                                m_nowAnimation.second,
                                                m_pCom3DModel->m_animationData);
            }
            return true;
        }
    }
    return false;
}

//================================================================================================
//================================================================================================

void Com3DAnimationAssimp::ResetFrame()
{
    m_pCom3DModel->m_animationData.m_frame = 0;
}

//================================================================================================
//================================================================================================

void Com3DAnimationAssimp::ChangeAnimationFrame()
{
    //todo ここ実装まだなので追加すること

    m_frameCounter += static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());
}