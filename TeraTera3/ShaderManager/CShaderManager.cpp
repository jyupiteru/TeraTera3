/**
 * @file CShaderManager.cpp
 * @author jupiter ()
 * @brief CShaderManagerクラスの実装が書かれているcpp
 */

#include "CShaderManager.h"
#include "../WindowsSystem/Shader/Shader.h"
#include "../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"
#include "../../ThirdParty/ImGui/imgui.h"

CShaderManager *CShaderManager::m_instance = nullptr;


void CShaderManager::Create()
{
	m_instance = new CShaderManager();
}

//================================================================================================
//================================================================================================

void CShaderManager::Delete(bool _flag)
{
	if (_flag)
	{
		for (auto &itr : m_instance->m_pVertexShaders)
		{
			itr.second->Release();
		}
		m_instance->m_pVertexShaders.clear();

		for (auto &itr : m_instance->m_pPixelShaders)
		{
			itr.second->Release();
		}
		m_instance->m_pPixelShaders.clear();

		for (auto &itr : m_instance->m_pLayout)
		{
			itr.second->Release();
		}
		m_instance->m_pLayout.clear();

		delete m_instance;
		m_instance = nullptr;
	}
}

//================================================================================================
//================================================================================================

CShaderManager &CShaderManager::GetInstance()
{
	return *m_instance;
}

//================================================================================================
//================================================================================================

void CShaderManager::LoadVertexShader(std::string_view _vsfile, D3D11_INPUT_ELEMENT_DESC *_layout, unsigned int _layoutsize, std::string_view _foldername)
{
	std::string shadername = _foldername.data();
	shadername += "/";
	shadername += _vsfile.data();

	//すでに読み込んでいないか確認
	if (!m_pVertexShaders.contains(shadername))
	{
		std::string filename = "Shader/" + shadername;

		ID3D11VertexShader *vertexshader;
		ID3D11InputLayout *layout;

		//存在しないので頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
		bool sts = CreateVertexShader(CDirectXGraphics::GetInstance().GetDXDevice(), //デバイス
									  filename.c_str(),								 //シェーダーソースファイル
									  "main",										 //エントリー関数
									  "vs_5_0",										 //シェーダーモデル
									  _layout,										 //頂点レイアウト
									  _layoutsize,									 //エレメント数
									  &vertexshader,								 //頂点シェーダーオブジェクト
									  &layout);										 //頂点レイアウト

		if (sts)
		{
			m_pLayout[shadername] = layout;
			m_pVertexShaders[shadername] = vertexshader;
		}
		else
		{ //生成できてないのでエラー (存在しない可能性あり)
			MessageBox(nullptr, TEXT(filename.c_str()), TEXT("error"), MB_OK);
		}
	}
}

//================================================================================================
//================================================================================================

void CShaderManager::LoadPixelShader(std::string_view _psfile, std::string_view _foldername)
{

	std::string shadername = _foldername.data();
	shadername += "/";
	shadername += _psfile.data();

	if (!m_pPixelShaders.contains(shadername))
	{
		std::string filename = "Shader/" + shadername;

		ID3D11PixelShader *pixelshader;

		//存在しないのでピクセルシェーダーを生成
		bool sts = CreatePixelShader(CDirectXGraphics::GetInstance().GetDXDevice(), // デバイス
									 filename.c_str(),								//ピクセルシェーダーファイル名
									 "main",										//エントリー関数
									 "ps_5_0",										//シェーダーモデル
									 &pixelshader);									//ピクセルシェーダーオブジェクト
		if (sts)
		{
			m_pPixelShaders[shadername] = pixelshader;
		}
		else
		{
			MessageBox(nullptr, TEXT(filename.c_str()), TEXT("error"), MB_OK);
		}
	}
}

//================================================================================================
//================================================================================================

void CShaderManager::SetVertexShader(std::string key)
{
	//頂点フォーマットをセット
	CDirectXGraphics::GetInstance().GetImmediateContext()->IASetInputLayout(m_pLayout[key]);

	//頂点シェーダーをセット
	CDirectXGraphics::GetInstance().GetImmediateContext()->VSSetShader(m_pVertexShaders[key], nullptr, 0);
}

//================================================================================================
//================================================================================================

void CShaderManager::SetPixelShader(std::string key)
{
	//ピクセルシェーダーをセット
	CDirectXGraphics::GetInstance().GetImmediateContext()->PSSetShader(m_pPixelShaders[key], nullptr, 0);
}

//================================================================================================
//================================================================================================

ID3D11VertexShader *CShaderManager::GetVertexShaderData(std::string_view _shadername, std::string_view _foldername)
{
	std::string shadername = _foldername.data();
	shadername += "/";
	shadername += _shadername.data();

	if (m_pVertexShaders.contains(shadername.data()))
	{
		return m_pVertexShaders[shadername.data()];
	}
	return nullptr;
}

//================================================================================================
//================================================================================================

ID3D11InputLayout *CShaderManager::GetLayoutData(std::string_view _shadername, std::string_view _foldername)
{
	std::string shadername = _foldername.data();
	shadername += "/";
	shadername += _shadername.data();

	if (m_pPixelShaders.contains(shadername.data()))
	{
		return m_pLayout[shadername.data()];
	}
	return nullptr;
}

//================================================================================================
//================================================================================================

ID3D11PixelShader *CShaderManager::GetPixelShaderData(std::string_view _shadername, std::string_view _foldername)
{
	std::string shadername = _foldername.data();
	shadername += "/";
	shadername += _shadername.data();

	if (m_pLayout.contains(shadername.data()))
	{
		return m_pPixelShaders[shadername.data()];
	}
	return nullptr;
}

//================================================================================================
//================================================================================================

void CShaderManager::ImGuiDraw(unsigned int)
{
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