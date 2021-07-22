
#include	"../Utils/CommonPSVS.fx"


VS_OUTPUT main(	float4 Pos		: POSITION,
				float4 Normal	: NORMAL,
				float4 Color    : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	float4 N = Normal;
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	N = mul(N, World);			//かける

	output.Normal = N;

	output.Color = Color;

	//光源からの距離を計算する
	float4 lengthfromlight;
	lengthfromlight = mul(Pos, World);
	lengthfromlight = mul(lengthfromlight, viewFromLight);
	lengthfromlight = mul(lengthfromlight, projectionFromLight);
	output.LengthFromLight = lengthfromlight;

	//シャドウマップを参照するためのUV座標作成
	output.ShadowTex = mul(lengthfromlight, screenToUVCoord);
	
	return output;
}
