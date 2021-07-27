
#include "CTextureManager.h"
#include "../../WindowsSystem/CDirectXGraphics/CDirectXGraphics.h"
#include "../../WindowsSystem/Shader/Shader.h"

CTextureManager *CTextureManager::m_instance = nullptr;

void CTextureManager::Create()
{
	m_instance = new CTextureManager();
}

//================================================================================================
//================================================================================================

void CTextureManager::Delete(bool _flag)
{
	if (_flag)
	{
		//リソースを開放
		for (auto &itr : m_instance->m_pListTextureData)
		{
			if (itr.second->srv)
			{
				itr.second->srv->Release();
			}
			if (itr.second->textureData)
			{
				itr.second->textureData->Release();
			}
			delete itr.second;
		}
		m_instance->m_pListTextureData.clear();

		delete m_instance;
		m_instance = nullptr;
	}
}

//================================================================================================
//================================================================================================

CTextureManager &CTextureManager::GetInstance()
{
	return *m_instance;
}

//================================================================================================
//================================================================================================

bool CTextureManager::LoadTexture(std::string_view _texturename, std::string_view _foldername)
{
	std::string filename = _foldername.data();
	filename += _texturename.data();

	//読み込み済みではないか？
	if (!m_pListTextureData.contains(filename))

	{
		ID3D11Resource *texture;
		ID3D11ShaderResourceView *srv;

		// SRV生成
		bool sts = CreateSRVfromFile(filename.c_str(), //画像ファイル名
									 CDirectXGraphics::GetInstance().GetDXDevice(),
									 CDirectXGraphics::GetInstance().GetImmediateContext(),
									 &texture,
									 &srv);
		if (!sts)
		{
			MessageBox(nullptr, TEXT("CreateSRVfromFile error"), TEXT("error"), MB_OK);
			return false;
		}

		//リストを生成して格納
		m_pListTextureData[filename] = new tagTextureData();
		m_pListTextureData[filename]->srv = srv;
		m_pListTextureData[filename]->textureData = texture;

		return true;
	}
	return false;
}

//================================================================================================
//================================================================================================

bool CTextureManager::SetTexture(std::string_view _texturename, ID3D11ShaderResourceView *_srv, ID3D11Resource *_resource)
{
	std::string filename = "Assets/Textures/";
	filename += _texturename.data();

	//すでにセット済みでないか？
	if (!m_pListTextureData.contains(filename))
	{
		m_pListTextureData[filename] = new tagTextureData();
		m_pListTextureData[filename]->srv = _srv;
		m_pListTextureData[filename]->textureData = _resource;

		return true;
	}
	return false;
}

//================================================================================================
//================================================================================================

tagTextureData *CTextureManager::GetTextureData(std::string_view _texturename, std::string_view _foldername)
{
	std::string filename = _foldername.data();

	filename += _texturename.data();

	//読み込み済みか？
	if (m_pListTextureData.contains(filename))
	{
		return m_pListTextureData[filename];
	}
	return nullptr;
}