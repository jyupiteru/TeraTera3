
////
//	ポイントライトの計算を含めたピクセルシェーダー
//	法線必須
////

#include	"../Utils/CommonPSVS.fx"
#include	"../Utils/MathLight.fx"


float4 main(VS_OUTPUT input) : SV_Target
{

	//環境光がどれくらい影響があるか計算して格納
	float4 diffuseDirection = CalcLambertDiffuse(DirectionalLight.LightDirection, DirectionalLight.LightColor, input.Normal);

	//鏡面反射光を求める
	float4 specularDirection = CalcPhongSpecular(DirectionalLight.LightDirection,DirectionalLight.LightColor,input.WPos,input.Normal,EyePos ,5.0f);


	//ポイントライトからピクセルまでの光の方向ベクトルを計算
	float4 ligDirection = input.WPos - PointLight.LightPosition;
	ligDirection = normalize(ligDirection);

	//ポイントライトからの拡散反射光を計算
	float4 diffusePoint = CalcLambertDiffuse(ligDirection,PointLight.lightColor,input.Normal);

	//ポイントライトからの鏡面反射光を求める
	float4 specularPoint = CalcPhongSpecular(ligDirection,PointLight.lightColor,input.WPos,input.Normal,EyePos,5.0f);

	//ポイントライトとピクセルの距離を計算する
	float distance = length(input.WPos - PointLight.LightPosition);

	//影響率を計算 遠くなれば遠くなるほど小さくなる
	float affect = 1.0f - 1.0f/ PointLight.LightRange * distance;

	//0以下にならないように計算
	affect = max(0.0f,affect);

	//指数関数で計算
	affect = pow(affect,3.0f);

	//減衰率をかけて影響を弱める
	diffusePoint *= affect;

	specularPoint *= affect;


	float4 diffuseLig = diffuseDirection + diffusePoint;
	float4 specularLig = specularDirection + specularPoint;

	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	//マテリアル色?の取得
	float4 outcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	//オブジェクト色をかける
	outcol *= col;

	//鏡面反射光と環境光と環境光を足す
	float4 lig = diffuseLig + specularLig + Ambient;

	//光をかける
	outcol *= lig;

	//色を全部反映 ここいじれば透過できそうやけど未挑戦
	outcol.a = 1.0f;

	return outcol;
}