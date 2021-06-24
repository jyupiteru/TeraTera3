

#ifndef _HLSL_COMMON_

#define _HLSL_COMMON_

Texture2D g_Tex : register(t0);				// テクスチャ
SamplerState g_SamplerLinear : register(s0);	// サンプラー

cbuffer ConstantBufferWorld : register(b0)
{
	matrix World;
}

cbuffer ConstantBufferView : register(b1)
{
	matrix View;
}

cbuffer ConstantBufferProjection : register(b2)
{
	matrix Projection;
}

cbuffer ConstantBufferMaterial : register(b3)
{
	float4	ambientMaterial;
	float4	diffuseMaterial;
	float4	specularMaterial;
}

//ディレクションライト用の構造体
struct tagDirectionalLight
{
	//正規化済みの方向ベクトル
	float4 LightDirection;	

	//0.0~1.0fの変換済みの色情報
	float4 LightColor;
};

struct tagPointLight
{
	//ポイントライトの座標
	float4 lightPosition;

	//ポイントライトの色
	float4 lightColor;

	//ポイントライトの影響範囲
	float lightRange;
};

struct tagHemispireLight
{
	//空の色
	float3 skyColor;

	//地面の色
	float3 groundColor;

	//地面の法線
	float3 groundNormal;
};

cbuffer ConstantBufferLight : register(b4)
{
	//ディレクショナルライトの情報が入った変数
	tagDirectionalLight DirectionalLight;			// 正規化していない光の方向

	//ポイントライトの情報が入った変数
	tagPointLight PointLight;

	//視点位置
	float4 EyePos;

	//環境光の情報
	float4 Ambient;
}

cbuffer ConstantBufferViewPort : register(b5)
{
    float4 ViewportWidth; // ビューポート幅
    float4 ViewportHeight; // ビューポート高さ
}

cbuffer ConstantBufferBoneMatrix : register(b6)
{
	float4 AssimpModelColor;
	matrix BoneMatrix[400];
}

cbuffer ConstantBufferWipe : register(b7)
{
	float wipeSize;	//現在のワイプのサイズ 表示をさせない範囲

	float2 wipeVector;	//ワイプの方向(正規化済み)
	
	float wipeFlag;		//ワイプが開けるか、閉まるか決めるフラグ
}

//頂点シェーダー => ピクセルシェーダーに渡す構造体
struct VS_OUTPUT
{
	//座標
	float4 Pos : SV_POSITION;	//座標 変換済み
	float4 Color : COLOR0;		//色
	float2 Tex : TEXCOORD;		//UV
	float4 WPos : TEXCOORD1;	//ワールド座標
	float4 Normal : TEXCOORD2;	//法線 正規化まだ
};

#endif