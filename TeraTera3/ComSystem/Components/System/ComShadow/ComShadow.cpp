/**
 * @file ComShadow.cpp
 * @author jupiter ()
 * @brief ComShadowクラスの実装が記載されたcpp
 */

#include "ComShadow.h"
#include "../../../../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"

ComShadow *ComShadow::m_instance = nullptr;

void ComShadow::Init()
{
	if (m_instance == nullptr)
	{
		m_instance = this;

		//描画は一番初めにしないといけないのでこれで
		m_gameObject->m_drawLayer.SetValue(-1000);
	}
	else
	{ //複数読み込むことはないため
		this->m_gameObject->RemoveComponent<ComShadow>();
	}
}

//================================================================================================
//================================================================================================

void ComShadow::Uninit()
{
	m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComShadow::Ready()
{
	m_comShader = this->m_gameObject->GetComponent<ComShader>();
	if (m_comShader == nullptr)
	{
		m_comShader = m_gameObject->AddComponent<ComShader>();
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	unsigned int numElements = ARRAYSIZE(layout);

	m_comShader->LoadVertexShader("VSShadow.fx", layout, numElements);
	m_comShader->LoadPixelShader("PSShadow.fx");

	//ライトコンポーネントを取得
	m_comLight = GameObject::Find("Light")->GetComponent<ComLight>();
}

//================================================================================================
//================================================================================================

void ComShadow::Update()
{
}

//================================================================================================
//================================================================================================

void ComShadow::Draw()
{
	DrawShadowMap();
}

//================================================================================================
//================================================================================================

void ComShadow::SetDrawShadowFuction(std::string_view _objname, std::function<void(void)> _function)
{
	//まだセットしていないか？
	if (!m_listObjectDrawFunction.contains(_objname.data()))
	{
		m_listObjectDrawFunction[_objname.data()] = _function;
	}
}

//================================================================================================
//================================================================================================

void ComShadow::RemoveDrawFunction(std::string_view _objname)
{
	if (m_listObjectDrawFunction.contains(_objname.data()))
	{
		m_listObjectDrawFunction.erase(_objname.data());
	}
}

//================================================================================================
//================================================================================================

void ComShadow::DrawShadowMap()
{
	// ターゲットバッファクリア
	float ClearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// デバイスコンテキスト取得
	ID3D11DeviceContext *devcontext = CDirectXGraphics::GetInstance().GetImmediateContext();

	//ここ追加すること

	//影を描画したいオブジェクトの描画処理を行いテクスチャを生成
	for (auto itr : m_listObjectDrawFunction)
	{
		itr.second();
	}
}