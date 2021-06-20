
////
//	モデル用のでデフォルトのピクセルシェーダー
//	法線必須
////


#include	"../Utils/CommonPSVS.fx"
#include	"../Utils/MathLight.fx"


float4 main(VS_OUTPUT input) : SV_Target
{
	//環境光がどれくらい影響があるか計算して格納
	float4 diffuseLig = CalcLambertDiffuse(DirectionalLight.LightDirection, DirectionalLight.LightColor, input.Normal);
	diffuseLig *= diffuseMaterial;

	//鏡面反射光を求める
	float specularLig = CalcPhongSpecular(DirectionalLight.LightDirection, DirectionalLight.LightColor,input.WPos,input.Normal,EyePos ,5.0f);
	specularLig *= specularMaterial;

	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	//マテリアル色?の取得
	float4 outcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	//オブジェクト色をかける
	outcol *= col;

	//鏡面反射光と環境光を足す
	float4 lig = diffuseLig + specularLig + Ambient;

	//光をかける
	outcol *= lig;

	//色を全部反映 ここいじれば透過できそうやけど未挑戦
	outcol.a = 1.0f;

	return outcol;
}