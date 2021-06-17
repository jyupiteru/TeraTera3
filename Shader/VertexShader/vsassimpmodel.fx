///////////////////
//	Assimpのモデル専用のシェーダ
///////////////////

#include"../commonpsvs.fx"


VS_OUTPUT main(
	float4 Pos		:	POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	int4   BoneIndex : BONEINDEX,
	float4 BoneWeight : BONEWEIGHT
)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = Tex;

	float4 N = Normal;
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	N = mul(N, World);			// 法線ベクトルにワールド変換行列を掛けて方向を変える
	N = normalize(N);			// 正規化

	output.Normal = N;          //ピクセルシェーダーでつかうために格納

	output.Color = AssimpModelColor;

	return output;
}