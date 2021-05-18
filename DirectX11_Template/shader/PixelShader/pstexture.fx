#include	"../commonpsvs.fx"

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{
	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	//これで透過する
	texcol *= col;


	return texcol;
}