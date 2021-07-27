/**
 * @file ComWipe.cpp
 * @author jupiter ()
 * @brief ComWipeクラスの実装が記載されているcpp
 */

#include "ComWipe.h"
#include "../../../../System/CDirectXGraphics/CDirectXGraphics.h"
#include "../../../../Timer/CTimer.h"

void ComWipe::Init()
{
	//表示レイヤーの変更
	m_gameObject->m_drawLayer.SetValue(30);

	m_typeComponent.SetValue(E_TYPE_COMPONENT::OBJECT2D);
	this->LoadTexture("System/white.png");

	Com2DTexture::Init();

	ID3D11Device *device = CDirectXGraphics::GetInstance().GetDXDevice();

	// 定数バッファ作成
	bool sts = CreateConstantBuffer(
		device,
		sizeof(ConstantBufferWipe),
		&m_wideDataBuffer);
	if (!sts)
	{
		MessageBox(NULL, "tagWipeData error", "Error", MB_OK);
	}

	m_wipeSize.SetValue(0);
	m_wipeVector.SetValue(1, 1);
	m_wipeSpeed.SetValue(150);
	m_wipeFlag.SetValue(true);
}

//================================================================================================
//================================================================================================

void ComWipe::Uninit()
{
	Com2DTexture::Uninit();
}

//================================================================================================
//================================================================================================

void ComWipe::Ready()
{
	Com2DTexture::Ready();
	m_pShader->LoadPixelShader("PS2DWipe.fx", true);
}

//================================================================================================
//================================================================================================

void ComWipe::Update()
{
	m_wipeSize.AddValue(static_cast<float>(m_wipeSpeed.GetValue() * CTimer::GetInstance().m_deltaTime.GetValue()));
}

//================================================================================================
//================================================================================================

void ComWipe::Draw()
{
	//m_wipeData.pad = {0, 0, 0};
	m_wipeData.wipeSize = m_wipeSize.GetValue();
	std::tie(m_wipeData.wipeVector.x, m_wipeData.wipeVector.y) = m_wipeVector.GetValue();
	m_wipeData.wipeFlag = m_wipeFlag.GetValue();
	if (m_wipeData.wipeFlag == 0)
	{
		m_wipeData.wipeFlag = -1;
	}

	//方向を変更
	//m_wipeData.wipeVector.x *= -1;
	//m_wipeData.wipeVector.y *= -1;

	//シェーダにワイプに必要な情報をセット
	CDirectXGraphics::GetInstance().GetImmediateContext()->UpdateSubresource(m_wideDataBuffer, 0, nullptr, &m_wipeData, 0, 0);
	CDirectXGraphics::GetInstance().GetImmediateContext()->PSSetConstantBuffers(7, 1, &m_wideDataBuffer);

	Com2DTexture::Draw();
}
