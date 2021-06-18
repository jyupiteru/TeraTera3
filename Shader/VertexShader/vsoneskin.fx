///////////////////
//	Assimpのアニメーションモデル専用のシェーダ
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

	// 頂点ブレンドの処理
	float4x4 comb = (float4x4)0;
	for (int i = 0; i < 4; i++)
	{
		//親から自分までを計算して座標生成?
		comb += BoneMatrix[BoneIndex[i]] * BoneWeight[i];
	}

	output.Pos = mul(Pos, comb);

	output.Pos = mul(output.Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	float4 N = Normal;
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	N = mul(N, comb);			// 法線ベクトルにコンビネーション行列を掛けて方向を変える
	N = mul(N, World);			// 法線ベクトルにワールド変換行列を掛けて方向を変える

	output.Normal = N;          //ピクセルシェーダーでつかうために格納

	output.Color = AssimpModelColor;

	output.Tex = Tex;

	return output;
}