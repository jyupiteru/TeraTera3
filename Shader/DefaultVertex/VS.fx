

#include	"../Utils/CommonPSVS.fx"


VS_OUTPUT main(	float4 Pos		:	POSITION,
	  			float4 Normal 	:	NORMAL,
				float2 Tex 		:	TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = Tex;

	float4 N = Normal;
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	N = mul(N, World);
	
	output.Color.x = 1.0f;
	output.Color.y = 1.0f;
	output.Color.z = 1.0f;
	output.Color.a = 1.0f;

	output.Normal = N;

	return output;
}