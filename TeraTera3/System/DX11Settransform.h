#pragma once
#include <DirectXMath.h>
#include <tchar.h>

#include "Shader/Shader.h"
#include "CDirectXGraphics/CDirectXGraphics.h"
#include "Dx11mathutil/Dx11mathutil.h"
#include "Memory.h"

class DX11SetTransform
{
	DX11SetTransform() {}

public:
	DX11SetTransform(const DX11SetTransform &) = delete;
	DX11SetTransform &operator=(const DX11SetTransform &) = delete;
	DX11SetTransform(DX11SetTransform &&) = delete;
	DX11SetTransform &operator=(DX11SetTransform &&) = delete;

	static DX11SetTransform *GetInstance()
	{
		static DX11SetTransform instance;
		return &instance;
	}
	// 定数バッファ定義(ワールド変換行列)
	ALIGN16 struct ConstantBufferWorld
	{
		DirectX::XMMATRIX World; // ワールド変換行列
	};
	// 定数バッファ定義(ビュー変換行列)
	ALIGN16 struct ConstantBufferView
	{
		DirectX::XMMATRIX View; // ビュー変換行列
	};
	// 定数バッファ定義(毎フレーム設定)
	ALIGN16 struct ConstantBufferProjection
	{
		DirectX::XMMATRIX Projection; // プロジェクション変換行列
	};

	enum class TYPE
	{
		WORLD,
		VIEW,
		PROJECTION
	};

	void Uninit()
	{
		if (m_pConstantBufferWorld)
		{
			m_pConstantBufferWorld->Release();
			m_pConstantBufferWorld = nullptr;
		}
		if (m_pConstantBufferView)
		{
			m_pConstantBufferView->Release();
			m_pConstantBufferView = nullptr;
		}
		if (m_pConstantBufferProjection)
		{
			m_pConstantBufferProjection->Release();
			m_pConstantBufferProjection = nullptr;
		}
	}

	bool Init()
	{

		ID3D11Device *device = CDirectXGraphics::GetInstance().GetDXDevice();

		// コンスタントバッファ作成
		bool sts = CreateConstantBuffer(
			device,						 // デバイス
			sizeof(ConstantBufferWorld), // サイズ
			&m_pConstantBufferWorld);	 // コンスタントバッファ２
		if (!sts)
		{
			MessageBox(NULL, _T("CreateBuffer(constant buffer world) error"), _T("Error"), MB_OK);
			return false;
		}

		// コンスタントバッファ作成
		sts = CreateConstantBuffer(
			device,						// デバイス
			sizeof(ConstantBufferView), // サイズ
			&m_pConstantBufferView);	// コンスタントバッファ２
		if (!sts)
		{
			MessageBox(NULL, _T("CreateBuffer(constant buffer view) error"), _T("Error"), MB_OK);
			return false;
		}

		// コンスタントバッファ作成
		sts = CreateConstantBuffer(
			device,							  // デバイス
			sizeof(ConstantBufferProjection), // サイズ
			&m_pConstantBufferProjection);	  // コンスタントバッファ２
		if (!sts)
		{
			MessageBox(NULL, _T("CreateBuffer(constant buffer projection) error"), _T("Error"), MB_OK);
			return false;
		}
		return true;
	}

	void SetTransform(TYPE type, DirectX::XMFLOAT4X4 &matrix)
	{
		ALIGN16 DirectX::XMMATRIX mat;
		mat = DirectX::XMLoadFloat4x4(&matrix);

		auto context = CDirectXGraphics::GetInstance().GetImmediateContext();

		switch (type)
		{
		case TYPE::WORLD:
			m_CBWorld.World = XMMatrixTranspose(mat);
			context->UpdateSubresource(m_pConstantBufferWorld, 0, nullptr, &m_CBWorld, 0, 0);
			context->VSSetConstantBuffers(0, 1, &m_pConstantBufferWorld);
			context->PSSetConstantBuffers(0, 1, &m_pConstantBufferWorld);
			break;
		case TYPE::VIEW:
			m_CBView.View = XMMatrixTranspose(mat);
			context->UpdateSubresource(m_pConstantBufferView, 0, nullptr, &m_CBView, 0, 0);
			context->VSSetConstantBuffers(1, 1, &m_pConstantBufferView);
			context->PSSetConstantBuffers(1, 1, &m_pConstantBufferView);
			break;
		case TYPE::PROJECTION:
			m_CBProjection.Projection = XMMatrixTranspose(mat);
			context->UpdateSubresource(m_pConstantBufferProjection, 0, nullptr, &m_CBProjection, 0, 0);
			context->VSSetConstantBuffers(2, 1, &m_pConstantBufferProjection);
			context->PSSetConstantBuffers(2, 1, &m_pConstantBufferProjection);
			break;
		}
	}

private:
	ID3D11Buffer *m_pConstantBufferWorld = nullptr;		 // コンスタントバッファ2
	ID3D11Buffer *m_pConstantBufferView = nullptr;		 // コンスタントバッファ2
	ID3D11Buffer *m_pConstantBufferProjection = nullptr; // コンスタントバッファ2
	ConstantBufferWorld m_CBWorld;						 // b0
	ConstantBufferView m_CBView;						 // b1
	ConstantBufferProjection m_CBProjection;			 // b2
};