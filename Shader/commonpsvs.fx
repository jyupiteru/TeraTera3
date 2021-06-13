Texture2D g_Tex : register(t0);				// テクスチャ
SamplerState g_SamplerLinear : register(s0);	// サンプラー

//--------------------------------------------------------------------------------------
// コンスタントバッファ
//--------------------------------------------------------------------------------------

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

cbuffer ConstantBufferLight : register(b4)
{
	float4 LightDirection;			// 光の方向
	float4 EyePos;					// 視点位置
	float4 Ambient;
}

cbuffer ConstantBufferViewPort : register(b5)
{
    float4 ViewportWidth; // ビューポート幅
    float4 ViewportHeight; // ビューポート高さ
}

cbuffer ConstantBufferBoneMatrix : register(b6)
{
	matrix BoneMatrix[400];
}

//頂点シェーダー => ピクセルシェーダーに渡す構造体
struct VS_OUTPUT
{
	//座標
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD;
	float4 WPos : TEXCOORD1;
	float4 Normal : TEXCOORD2;
};

