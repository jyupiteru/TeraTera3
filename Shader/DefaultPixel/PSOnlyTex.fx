
#include	"../Utils/CommonPSVS.fx"


float4 main(VS_OUTPUT input) : SV_Target
{
	//テクスチャの色を取得
	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	//テクスチャの色をそのまま反映
	float4 col = texcol;
	col.a = 1.0f;
	return col;
}