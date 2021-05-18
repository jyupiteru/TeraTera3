#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include "memory.h"
#include "../../../CDirectXGraphics/CDirectXGraphics.h"
#include "../../../Shader/Shader.h"
#include "../vertexproto.h"
#include "../Mesh/mesh.h"
#include "../../../Dx11mathutil/Dx11mathutil.h"
#include "../../../Dx11util/DX11util.h"

using namespace std;
using namespace DirectX;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mtrl) {
	this->m_vertices = vertices;
	this->m_indices = indices;
	//	this->m_dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	this->m_dev = GetDX11Device();
	this->m_textures = textures;
	this->m_mtrl = mtrl;
	this->setupMesh();
	//		this->m_deformvertices.resize(vertices.size());			// GPU�Ή��ŕs�v


	std::vector<XMFLOAT3> tempv;

	// �{�[����o�^
	for (int i = 0; i < vertices.size(); i++) {
		for (int j = 0; j < 4; j++) {
			if (vertices[i].m_BoneName[j] != "") {
				m_bonevertices[vertices[i].m_BoneName[j]] = tempv;
			}
		}
	}

	// ���_��o�^
	for (int i = 0; i < vertices.size(); i++) {
		for (int j = 0; j < 4; j++) {
			// �{�[�����݃`�F�b�N
			auto itr = m_bonevertices.find(vertices[i].m_BoneName[j].c_str());
			if (itr != m_bonevertices.end()) {
				std::vector<XMFLOAT3>& vv = m_bonevertices[vertices[i].m_BoneName[j].c_str()];
				vv.push_back(vertices[i].m_Pos);
			}
		}
	}
}

void Mesh::Draw(ID3D11DeviceContext *devcon)
{
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;
		// ���_�o�b�t�@���Z�b�g
		devcon->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		// �C���f�b�N�X�o�b�t�@���Z�b�g
		devcon->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// �g�|���W�[���Z�b�g
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// SRV���Z�b�g
		if (m_textures.size() >= 1) {
			devcon->PSSetShaderResources(0, 1, &m_textures[0].texture);
		}
		else {
			//devcon->PSSetShaderResources(0, 1, &CDirectXGraphics::m_whitesrv);
		}
		// �}�e���A����PS��VS�ւ�����
		devcon->PSSetConstantBuffers(3, 1, &m_cbmtrl);
		devcon->VSSetConstantBuffers(3, 1, &m_cbmtrl);

		// �C���f�b�N�X�o�b�t�@�𗘗p���ĕ`��
		devcon->DrawIndexed(static_cast<unsigned int>(m_indices.size()), 0, 0);
}

void Mesh::Close()
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
}

bool Mesh::setupMesh()
{
		// ���_�o�b�t�@����
		bool sts = CreateVertexBufferWrite(m_dev,
		static_cast<unsigned int>(sizeof(Vertex)),			// �X�g���C�h
		static_cast<unsigned int>(m_vertices.size()),		// ���_��
		m_vertices.data(),									// ���_�f�[�^
		&m_VertexBuffer);
		if (!sts) {
			return false;
		}

		// �C���f�b�N�X�o�b�t�@����
		sts = CreateIndexBuffer(m_dev,
			static_cast<unsigned int>(m_indices.size()),
			m_indices.data(),
			&m_IndexBuffer);
		if (!sts) {
			return false;
		}

		// �}�e���A���p�R���X�^���g�o�b�t�@����
		sts = CreateConstantBufferWrite(m_dev,
			sizeof(ConstantBufferMaterial),
			&m_cbmtrl);
		if (!sts) {
			return false;
		}

		ID3D11DeviceContext* devicecontext;
		//	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		devicecontext = GetDX11DeviceContext();

		// �萔�o�b�t�@�X�V
		D3D11_MAPPED_SUBRESOURCE pData;

		ConstantBufferMaterial cb;

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
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferMaterial));
			devicecontext->Unmap(m_cbmtrl, 0);
		}

		return true;
}

void Mesh::updatevertexbuffer() {

	HRESULT hr;

	ID3D11DeviceContext* devicecontext;

//	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	devicecontext = GetDX11DeviceContext();

	D3D11_MAPPED_SUBRESOURCE pData;

	// �萔�o�b�t�@�X�V
	hr = devicecontext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		//			memcpy_s(pData.pData, pData.RowPitch, (void*)m_deformvertices.data(), sizeof(aiVertex)*m_deformvertices.size());
		memcpy_s(pData.pData, pData.RowPitch, (void*)m_vertices.data(), sizeof(Vertex)*m_vertices.size());
		devicecontext->Unmap(m_VertexBuffer, 0);
	}
}