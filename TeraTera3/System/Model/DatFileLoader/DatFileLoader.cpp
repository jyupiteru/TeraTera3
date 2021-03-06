#include "DatFileLoader.h"

DatFileLoader::~DatFileLoader()
{
	if (m_Subset)
	{
		delete[] m_Subset;
		m_Subset = nullptr;
	}
	if (m_DecalTex)
	{
		delete[] m_DecalTex;
		m_DecalTex = nullptr;
	}
	if (m_Material)
	{
		delete[] m_Material;
		m_Material = nullptr;
	}
	if (m_Index)
	{
		delete[] m_Index;
		m_Index = nullptr;
	}
	if (m_Vertex)
	{
		delete[] m_Vertex;
		m_Vertex = nullptr;
	}
}

void DatFileLoader::Serialize(const char *filename)
{

	FILE *fp;

	fopen_s(&fp, filename, "w");

	// 頂点数
	fprintf(fp, "%d\n", m_VertexSuu);
	for (int i = 0; i < m_VertexSuu; i++)
	{
		fprintf(fp, "%f %f %f \n", m_Vertex[i].m_Pos.x, m_Vertex[i].m_Pos.y, m_Vertex[i].m_Pos.z);
		fprintf(fp, "%f %f %f \n", m_Vertex[i].m_Normal.x, m_Vertex[i].m_Normal.y, m_Vertex[i].m_Normal.z);
		fprintf(fp, "%f %f \n", m_Vertex[i].m_Tex.x, m_Vertex[i].m_Tex.y);
	}

	// インデックス数
	fprintf(fp, "%d\n", m_IndexSuu);
	for (int i = 0; i < m_IndexSuu; i++)
	{
		fprintf(fp, "%d\n", m_Index[i]);
	}

	// マテリアル数
	fprintf(fp, "%d\n", m_MaterialSuu);
	for (int i = 0; i < m_MaterialSuu; i++)
	{
		fprintf(fp, "%f \n", m_Material[i].m_Alpha);
		fprintf(fp, "%f %f %f \n", m_Material[i].m_Diffuse.x, m_Material[i].m_Diffuse.y, m_Material[i].m_Diffuse.z);
		fprintf(fp, "%f %f %f \n", m_Material[i].m_Specular.x, m_Material[i].m_Specular.y, m_Material[i].m_Specular.z);
		fprintf(fp, "%f \n", m_Material[i].m_SpecularPower);

		fprintf(fp, "%f %f %f \n", m_Material[i].m_Ambient.x, m_Material[i].m_Ambient.y, m_Material[i].m_Ambient.z);
		fprintf(fp, "%f %f %f \n", m_Material[i].m_Emissive.x, m_Material[i].m_Emissive.y, m_Material[i].m_Emissive.z);

		if (m_DecalTex->size() != 0)
		{
			fprintf(fp, "%s \n", m_DecalTex->c_str());
		}
		else
		{
			fprintf(fp, "%s \n", "null");
		}
		m_DecalTex++;
	}

	// サブセット数
	fprintf(fp, "%d\n", m_SubsetSuu);
	for (int i = 0; i < m_SubsetSuu; i++)
	{
		fprintf(fp, "%d \n", m_Subset[i].m_MaterialIdx);
		fprintf(fp, "%d \n", m_Subset[i].m_VertexSuu);

		fprintf(fp, "%d \n", m_Subset[i].m_FaceStart);
		fprintf(fp, "%d \n", m_Subset[i].m_FaceCount);

		fprintf(fp, "%d \n", m_Subset[i].m_VertexStart);
		fprintf(fp, "%d \n", m_Subset[i].m_VertexCount);
	}

	fclose(fp);
}

bool DatFileLoader::Deserialize(const char *filename)
{

	FILE *fp;

	errno_t err = fopen_s(&fp, filename, "r");
	if (err != 0)
	{
		return false;
	}

	// 頂点数
	fscanf_s(fp, "%d\n", &m_VertexSuu);
	m_Vertex = new X::Vertex[m_VertexSuu];

	for (int i = 0; i < m_VertexSuu; i++)
	{
		fscanf_s(fp, "%f %f %f ", &m_Vertex[i].m_Pos.x, &m_Vertex[i].m_Pos.y, &m_Vertex[i].m_Pos.z);
		fscanf_s(fp, "%f %f %f ", &m_Vertex[i].m_Normal.x, &m_Vertex[i].m_Normal.y, &m_Vertex[i].m_Normal.z);
		fscanf_s(fp, "%f %f ", &m_Vertex[i].m_Tex.x, &m_Vertex[i].m_Tex.y);
	}

	// インデックス数
	fscanf_s(fp, "%d\n", &m_IndexSuu);

	m_Index = new X::Index[m_IndexSuu];
	for (int i = 0; i < m_IndexSuu; i++)
	{
		fscanf_s(fp, "%d", &m_Index[i]);
	}

	// マテリアル数
	fscanf_s(fp, "%d", &m_MaterialSuu);

	m_Material = new X::Material[m_MaterialSuu];

	m_DecalTex = new std::string[m_MaterialSuu];

	for (int i = 0; i < m_MaterialSuu; i++)
	{
		fscanf_s(fp, "%f ", &m_Material[i].m_Alpha);
		fscanf_s(fp, "%f %f %f ", &m_Material[i].m_Diffuse.x, &m_Material[i].m_Diffuse.y, &m_Material[i].m_Diffuse.z);
		fscanf_s(fp, "%f %f %f ", &m_Material[i].m_Specular.x, &m_Material[i].m_Specular.y, &m_Material[i].m_Specular.z);
		fscanf_s(fp, "%f ", &m_Material[i].m_SpecularPower);

		fscanf_s(fp, "%f %f %f ", &m_Material[i].m_Ambient.x, &m_Material[i].m_Ambient.y, &m_Material[i].m_Ambient.z);
		fscanf_s(fp, "%f %f %f ", &m_Material[i].m_Emissive.x, &m_Material[i].m_Emissive.y, &m_Material[i].m_Emissive.z);

		char data[256];
		fscanf_s(fp, "%s", data, 256);

		std::string *tempstr;
		tempstr = new std::string(data);

		m_DecalTex[i] = *tempstr;

		delete tempstr;
	}

	// サブセット数
	fscanf_s(fp, "%d", &m_SubsetSuu);

	m_Subset = new X::Subset[m_SubsetSuu];

	for (int i = 0; i < m_SubsetSuu; i++)
	{
		fscanf_s(fp, "%d ", &m_Subset[i].m_MaterialIdx);
		fscanf_s(fp, "%d ", &m_Subset[i].m_VertexSuu);

		fscanf_s(fp, "%d ", &m_Subset[i].m_FaceStart);
		fscanf_s(fp, "%d ", &m_Subset[i].m_FaceCount);

		fscanf_s(fp, "%d ", &m_Subset[i].m_VertexStart);
		fscanf_s(fp, "%d ", &m_Subset[i].m_VertexCount);
	}

	fclose(fp);

	return true;
}
