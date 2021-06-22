
////
//	ランバートの拡散反射のピクセルシェーダー
//	法線必須
////

#include "../Utils/CommonPSVS.fx"
#include "../Utils/MathLight.fx"

float4 main(VS_OUTPUT input) : SV_Target
{

	//環境光がどれくらい影響があるか計算して格納
	float4 diffuseLig = CalcLambertDiffuse(LightDirection, Ambient, input.Normal);

	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	//マテリアル色?の取得
	float4 outcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	//オブジェクト色をかける
	outcol *= col;

	//環境光をかける
	outcol *= diffuseLig;

	//色を全部反映 ここいじれば透過できそうやけど未挑戦
	outcol.a = 1.0f;

	return outcol;
}