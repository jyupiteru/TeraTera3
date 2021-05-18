#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp/cimport.h>
#include <unordered_map>
#include <vector>
#include "../animationdataassimp/animationdataassimp.h"
#include "../../../Shader/Shader.h"
#include "memory.h"
#include "ModelData.h"
#include "../vertexproto.h"
#include "../../../Dx11util/DX11util.h"
#include "../../../DX11Settransform.h"
#include "../../../Dx11mathutil/Dx11mathutil.h"

const int INTERPOLATENUM = 3; // 補間数

// マテリアルに関連してるテクスチャを取得する
std::vector<Texture> ModelData::loadMaterialTextures(
	aiMaterial *mtrl,
	aiTextureType type,
	std::string typeName,
	const aiScene *scene)
{
	std::vector<Texture> textures; // このマテリアルに関連づいたDIFFUSEテクスチャのリスト
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;

	dev = GetDX11Device();
	devcon = GetDX11DeviceContext();

	// マテリアルからテクスチャ個数を取得し(基本は1個)ループする
	for (unsigned int i = 0; i < mtrl->GetTextureCount(type); i++)
	{
		aiString str;

		// マテリアルからｉ番目のテクスチャファイル名を取得する
		mtrl->GetTexture(type, i, &str);

		// もし既にロードされたテクスチャであればロードをスキップする
		bool skip = false;

		// ロード済みテクスチャ数分ループする
		for (unsigned int j = 0; j < m_texturesloaded.size(); j++)
		{
			// ファイル名が同じであったら読み込まない
			if (std::strcmp(m_texturesloaded[j].path.c_str(), str.C_Str()) == 0)
			{
				// 読み込み済みのテクスチャ情報をDIFFUSEテクスチャのリストにセット
				textures.push_back(m_texturesloaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{ // まだ読み込まれていなかった場合
			Texture tex;

			std::string filename = std::string(str.C_Str());
			filename = ExtractFileName(filename, "\\/"); // ファイル名を取得
			filename = m_directory + filename;			 // リソースディレクトリ＋ファイル名
														 // ts			MessageBox(nullptr, filename.c_str(), "load tex", MB_OK);

			// SRV生成
			bool sts = CreateSRVfromFile(
				filename.c_str(),
				dev, devcon, &tex.resource, &tex.texture);
			if (!sts)
			{
				MessageBox(nullptr, "Texture couldn't be loaded", filename.c_str(), MB_ICONERROR | MB_OK);
			}

			tex.type = typeName;
			tex.path = str.C_Str();
			// テクスチャ情報をDIFFUSEテクスチャのリストにセット
			textures.push_back(tex);
			this->m_texturesloaded.push_back(tex); // このモデルに関連づいたテクスチャリストにセット
		}
	}

	return textures;
}

ModelData::ModelData()
{
}

ModelData::~ModelData()
{
	Exit();
}

// マテリアルデータを取得する
void ModelData::LoadMaterial()
{

	// マテリアルが存在するか？
	if (m_assimpscene.GetScene()->HasMaterials())
	{
		// マテリアル数 取得
		int nummaterial = m_assimpscene.GetScene()->mNumMaterials;

		for (int i = 0; i < nummaterial; i++)
		{
			Material mtrl;

			// i番目のマテリアルを取得
			aiMaterial *mat = m_assimpscene.GetScene()->mMaterials[i];

			aiColor3D colordiffuse(1.f, 1.f, 1.f);
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, colordiffuse);
			mtrl.m_Diffuse.x = colordiffuse.r;
			mtrl.m_Diffuse.y = colordiffuse.g;
			mtrl.m_Diffuse.z = colordiffuse.b;
			//			mtrl.m_Diffuse.x = 1.0f;					// マテリアル値が0の場合があるので
			//			mtrl.m_Diffuse.y = 1.0f;					// マテリアル値が0の場合があるので
			//			mtrl.m_Diffuse.z = 1.0f;					// マテリアル値が0の場合があるので

			aiColor3D colorspecular(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_SPECULAR, colorspecular);
			mtrl.m_Specular.x = colorspecular.r;
			mtrl.m_Specular.y = colorspecular.g;
			mtrl.m_Specular.z = colorspecular.b;

			aiColor3D colorambient(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_AMBIENT, colorambient);
			mtrl.m_Ambient.x = colorambient.r;
			mtrl.m_Ambient.y = colorambient.g;
			mtrl.m_Ambient.z = colorambient.b;

			aiColor3D coloremissive(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_EMISSIVE, coloremissive);
			mtrl.m_Emissive.x = coloremissive.r;
			mtrl.m_Emissive.y = coloremissive.g;
			mtrl.m_Emissive.z = coloremissive.b;

			float power;
			mat->Get(AI_MATKEY_SHININESS_STRENGTH, power);
			mtrl.m_SpecularPower = power;

			m_materials.push_back(mtrl);
		}
	}
}

// ボーン名ＭＡＰを生成
void ModelData::CreateBone(aiNode *node)
{
	BONE bone;

	/*
		struct BONE
		{
			//std::string Name;
			aiMatrix4x4 Matrix;						// 初期配置行列
			aiMatrix4x4 AnimationMatrix;			// ボーン行列
			aiMatrix4x4 OffsetMatrix;				// ボーンオフセット行列
		};

		std::map<std::string, BONE> m_Bone;			//ボーンデータ（名前で参照）
	*/

	// ノード名をキーにしてボーン情報を保存
	m_Bone[node->mName.C_Str()] = bone;

	// 子供の数分ループしてボーン名ＭＡＰを生成
	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

bool ModelData::Load(std::string resourcefolder,
					 std::string filename)
{
	bool sts = m_assimpscene.Init(filename);
	if (!sts)
	{
		MessageBox(nullptr, "ModelData load error", "error", MB_OK);
		return false;
	}

	// このモデルのテクスチャが存在するディレクトリ
	m_directory = resourcefolder;

	// ボーンマップを生成する
	CreateBone(m_assimpscene.GetScene()->mRootNode);

	// ボーンの配列位置を格納する
	unsigned int num = 0;
	for (auto &data : m_Bone)
	{
		data.second.idx = num;
		num++;
	}

	// マテリアルを取得
	LoadMaterial();

	// ａｓｓｉｍｐノードを解析する
	processNode(m_assimpscene.GetScene()->mRootNode, m_assimpscene.GetScene());

	// ボーン行列格納用の定数バッファを生成する
	ID3D11Device *device;
	device = GetDX11Device();

	sts = CreateConstantBufferWrite(device, sizeof(ConstantBufferBoneMatrix), &m_constantbufferbonematrix);
	if (!sts)
	{
		MessageBox(nullptr, "constant buffer create(bonematrix) fail", "error", MB_OK);
		return false;
	}

	return true;
}

// ３Ｄモデル描画
void ModelData::Draw(ID3D11DeviceContext *devcon, DirectX::XMFLOAT4X4 &mtxworld,
					 tagAssimpAnimationData &animationdata)
{
	// アニメーションデータを持っているか？
	if (m_assimpscene.HasAnimation())
	{ // ボーン行列を定数バッファに反映させる
		UpdateBoneMatrixConstantBuffer(animationdata.m_listBone);
	}

	// メッシュ数分ループしてモデルを描画する
	for (int i = 0; i < m_meshes.size(); i++)
	{
		// ワールド変換行列
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtxworld);
		// 定数バッファセット処理
		m_meshes[i].Draw(devcon);
	}
}

void ModelData::ChangeAnimation(const aiScene *animationscene,
								unsigned int animno,
								tagAssimpAnimationData &animationdata)
{
	// アニメーションデータを持っているか？
	if (animationscene->HasAnimations())
	{
		// アニメーションデータ取得
		aiAnimation *animation = animationscene->mAnimations[animno];

		//一番初めはblenderとかのツールの一番上の全体のやつなので取得
		auto firstnode = animation->mChannels[0];

		animationdata.m_maxflame = firstnode->mNumPositionKeys;
		if (animationdata.m_maxflame < firstnode->mNumRotationKeys)
		{
			animationdata.m_maxflame = firstnode->mNumRotationKeys;
		}

		if (animationdata.m_maxflame < firstnode->mNumScalingKeys)
		{
			animationdata.m_maxflame = firstnode->mNumScalingKeys;
		}

		//1秒当たりなんフレームかを取得
		animationdata.m_TickPerSecond = static_cast<float>(animation->mTicksPerSecond);
	}
}
// メッシュの解析
Mesh ModelData::processMesh(aiMesh *mesh, const aiScene *scene, int meshidx)
{
	std::vector<Vertex> vertices;	   // 頂点
	std::vector<unsigned int> indices; // 面の構成情報
	std::vector<Texture> textures;	   // テクスチャ
	Material mtrl;

	// 頂点情報を取得
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.m_Pos.x = mesh->mVertices[i].x;
		vertex.m_Pos.y = mesh->mVertices[i].y;
		vertex.m_Pos.z = mesh->mVertices[i].z;

		// 法線ベクトルが存在するか？
		if (mesh->HasNormals())
		{
			vertex.m_Normal.x = mesh->mNormals[i].x;
			vertex.m_Normal.y = mesh->mNormals[i].y;
			vertex.m_Normal.z = mesh->mNormals[i].z;
		}
		else
		{
			vertex.m_Normal.x = 0.0f;
			vertex.m_Normal.y = 0.0f;
			vertex.m_Normal.z = 0.0f;
		}

		// テクスチャ座標（０番目）が存在するか？
		if (mesh->HasTextureCoords(0))
		{
			vertex.m_Tex.x = mesh->mTextureCoords[0][i].x;
			vertex.m_Tex.y = mesh->mTextureCoords[0][i].y;
		}

		vertex.m_BoneNum = 0;

		for (unsigned int b = 0; b < 4; b++)
		{
			vertex.m_BoneIndex[b] = -1;
			vertex.m_BoneName[b] = "";
			vertex.m_BoneWeight[b] = 0.0f;
		}

		vertices.push_back(vertex);
	}

	//ボーンデータ初期化
	for (unsigned int b = 0; b < mesh->mNumBones; b++)
	{
		// メッシュに関連づいてるボーン情報を取得
		aiBone *bone = mesh->mBones[b];

		// ボーンオフセット行列を取得
		m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

		// ボーンに関連づいている頂点を選び､ウェイト値をセットする
		for (unsigned int widx = 0; widx < bone->mNumWeights; widx++)
		{
			aiVertexWeight weight = bone->mWeights[widx];

			unsigned int vidx = weight.mVertexId; // このウエイトに関連づいてる頂点idx

			// メッシュの中の何番目か
			vertices[vidx].m_BoneWeight[vertices[vidx].m_BoneNum] = weight.mWeight;
			vertices[vidx].m_BoneName[vertices[vidx].m_BoneNum] = bone->mName.C_Str();
			// 該当するボーン名のインデックス値をセット
			vertices[vidx].m_BoneIndex[vertices[vidx].m_BoneNum] = m_Bone[bone->mName.C_Str()].idx;

			vertices[vidx].m_BoneNum++;

			assert(vertices[vidx].m_BoneNum <= 4);
		}
	}

	/*
	FILE* fp;
	fopen_s(&fp, "vertex,txt", "w");

	for (auto v : vertices) {
		fprintf_s(fp, "(%d) x:%f y:%f z:%f ", v.m_Pos.x, v.m_Pos.y, v.m_Pos.z);
		fprintf_s(fp, "(%d) bone w1:%f w2:%f w3:%f w4:%f", v.m_BoneWeight[0], v.m_BoneWeight[1], v.m_BoneWeight[2], v.m_BoneWeight[3]);
		fprintf_s(fp, "(%d) bone idx1:%d,idx2:%d,idx3;%d );
	}

	fclose(fp);

	*/

	// テクスチャ情報を取得する
	if (mesh->mMaterialIndex >= 0)
	{
		// このメッシュのマテリアルインデックス値を取得する
		int mtrlidx = mesh->mMaterialIndex;

		// このメッシュのマテリアルを取得する
		mtrl = m_materials[mtrlidx];

		// シーンからマテリアルデータを取得する
		aiMaterial *material = scene->mMaterials[mtrlidx];

		// このマテリアルに関連づいたテクスチャを取り出す
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);

		// このメッシュで使用しているテクスチャを保存
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	// 面の構成情報を取得
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices, textures, mtrl);
}

void ModelData::UpdateBoneMatrix(aiNode *node, aiMatrix4x4 matrix, std::map<std::string, BONE> &bonedata)
{
	// ボーンＭＡＰから該当ボーンアドレスを取得（ｂｏｎｅ名でボーンを取得）
	BONE *bone = &bonedata[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	//（親ボーンとの相対位置を示す行列＊ボーン行列＊ボーンオフセット行列）
	aiMatrix4x4 worldMatrix;
	//
	/*
	TReal a1, a2, a3, a4;   // assimp 行列は、行優先
	TReal b1, b2, b3, b4;
	TReal c1, c2, c3, c4;
	TReal d1, d2, d3, d4;
	*/

	worldMatrix = matrix;				  // 初期配置行列
	worldMatrix *= bone->AnimationMatrix; // 親座標系＊ボーン行列

	bone->Matrix = worldMatrix;			// 初期配置行列×ボーン行列
	bone->Matrix *= bone->OffsetMatrix; // 初期配置行列×ボーン行列×ボーンオフセット行列

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix, bonedata);
	}
}

void ModelData::Update(const DirectX::XMFLOAT4X4 &mtxworld,
					   const aiScene *animationscene,
					   unsigned int animno,
					   tagAssimpAnimationData &animationdata)
{ // アニメーションコンテナ

	// // アニメーションファイル番号に該当するシーン情報を取得する（アニメーションデータ）
	// const aiScene *s = animationcontainer[animfileno]->GetScene();

	// アニメーションデータを持っているか？
	if (animationscene->HasAnimations())
	{
		//アニメーションデータからボーンマトリクス算出

		// アニメーションデータ取得
		aiAnimation *animation = animationscene->mAnimations[animno];

		// ボーンの数だけループ
		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim *nodeAnim = animation->mChannels[c];

			// ボーン存在チェック(アニメーションにのみ存在するボーンがあった場合は無視する)
			auto itr = animationdata.m_listBone.find(nodeAnim->mNodeName.C_Str());
			if (itr != animationdata.m_listBone.end())
			{
				BONE *bone = &animationdata.m_listBone[nodeAnim->mNodeName.C_Str()];

				int f1, f2;

				f1 = animationdata.m_frame % nodeAnim->mNumRotationKeys; //簡易実装
				aiQuaternion rot1 = nodeAnim->mRotationKeys[f1].mValue;	 // クオータニオン

				f1 = animationdata.m_frame % nodeAnim->mNumPositionKeys; //簡易実装
				aiVector3D pos1 = nodeAnim->mPositionKeys[f1].mValue;

				f2 = animationdata.m_preFrame % nodeAnim->mNumRotationKeys; //簡易実装
				aiQuaternion rot2 = nodeAnim->mRotationKeys[f2].mValue;		// クオータニオン

				f2 = animationdata.m_preFrame % nodeAnim->mNumPositionKeys; //簡易実装
				aiVector3D pos2 = nodeAnim->mPositionKeys[f2].mValue;

				// 補間
				rot1.Interpolate(rot1, rot1, rot2, animationdata.m_factor);
				pos1.x = pos1.x * (1.0f - animationdata.m_factor) + pos2.x * (animationdata.m_factor);
				pos1.y = pos1.y * (1.0f - animationdata.m_factor) + pos2.y * (animationdata.m_factor);
				pos1.z = pos1.z * (1.0f - animationdata.m_factor) + pos2.z * (animationdata.m_factor);

				// ボーン行列を更新
				bone->AnimationMatrix = aiMatrix4x4(
					aiVector3D(1.0f, 1.0f, 1.0f), // スケール値
					rot1,						  // 姿勢（クオータニオン）
					pos1);						  // 位置

				if (animationdata.m_maxflame < nodeAnim->mNumRotationKeys)
				{
					animationdata.m_maxflame = nodeAnim->mNumRotationKeys;
				}
			}
		}

		//再帰的にボーンマトリクスを更新
		UpdateBoneMatrix(
			m_assimpscene.GetScene()->mRootNode, // モデルデータのルートノードを指定
			aiMatrix4x4(),						 // 	ワールド変換行列を頂点シェーダーで処理しているので単位行列を渡す
			animationdata.m_listBone);
	}

	if (animationdata.m_cnt % INTERPOLATENUM == 0)
	{
		animationdata.m_preFrame = animationdata.m_frame;
		animationdata.m_frame++;
		animationdata.m_factor = 0;
	}

	animationdata.m_factor = 1.0f / (float)(animationdata.m_cnt % INTERPOLATENUM + 1);

	animationdata.m_cnt++;
}

void ModelData::Exit()
{
	// テクスチャリソースを解放する
	for (auto tex : m_texturesloaded)
	{
		if (tex.texture != nullptr)
		{
			tex.texture->Release();
			tex.resource->Release();
		}
	}

	// メッシュの解放
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Close();
	}

	// 定数バッファ解放
	if (m_constantbufferbonematrix)
	{
		m_constantbufferbonematrix->Release();
		m_constantbufferbonematrix = nullptr;
	}

	// assimp scene 解放
	m_assimpscene.Exit();

	// シーン(アニメーション用の)の解放
	/*for (auto s : m_sceneAnimContainer)
	{
		aiReleaseImport(s);
	}*/
}

// ノードの解析
void ModelData::processNode(aiNode *node, const aiScene *scene)
{
	// ノード内のメッシュの数分ループする
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int meshindex = node->mMeshes[i];		  // ノードのi番目メッシュのインデックスを取得
		aiMesh *mesh = scene->mMeshes[meshindex]; // シーンからメッシュ本体を取り出す

		m_meshes.push_back(this->processMesh(mesh, scene, meshindex));
	}

	// 子ノードについても解析
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

// ボーン定数バッファ更新
void ModelData::UpdateBoneMatrixConstantBuffer(std::map<std::string, BONE> &listbone)
{

	ConstantBufferBoneMatrix cb;
	DirectX::XMFLOAT4X4 mtx;

	// ボーン行列を定数バッファにセット
	unsigned int num = 0;
	for (auto data : listbone)
	{
		// aiMatrix4X4をXMFLOAT4X4へ変換(転置する)
		mtx = DX11MtxaiToDX(data.second.Matrix);

		// 転置する
		DX11MtxTranspose(mtx, mtx);

		cb.mBoneMatrix[num] = mtx;
		num++;
	}

	ID3D11DeviceContext *devicecontext;
	devicecontext = GetDX11DeviceContext();

	D3D11_MAPPED_SUBRESOURCE pData;

	// ボーン行列格納用定数バッファ更新
	HRESULT hr = devicecontext->Map(m_constantbufferbonematrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, (void *)&cb, sizeof(ConstantBufferBoneMatrix));
		devicecontext->Unmap(m_constantbufferbonematrix, 0);
	}

	// GPUへ定数バッファをセット
	devicecontext->VSSetConstantBuffers(6, 1, &m_constantbufferbonematrix);
}
