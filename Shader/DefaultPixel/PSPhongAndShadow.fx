
////
//	フォングの鏡面反射のピクセルシェーダー
//	法線必須
////


#include	"../Utils/CommonPSVS.fx"
#include	"../Utils/MathLight.fx"


float4 main(VS_OUTPUT input) : SV_Target
{

	//環境光がどれくらい影響があるか計算して格納
	float4 diffuseLig = CalcLambertDiffuse(DirectionalLight.LightDirection, DirectionalLight.LightColor, input.Normal);

	//鏡面反射光を求める
	float4 specularLig = CalcPhongSpecular(DirectionalLight.LightDirection, DirectionalLight.LightColor,input.WPos,input.Normal,EyePos ,5.0f);

	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	//マテリアル色?の取得
	float4 outcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	//オブジェクト色をかける
	outcol *= col;

	//鏡面反射光と環境光を足す
	float4 lig = diffuseLig + specularLig;

	//光をかける
	outcol *= lig;

	//色を全部反映 ここいじれば透過できそうやけど未挑戦
	outcol.a = 1.0f;

	
	// シャドウマップから遮蔽物までの距離を取り出す
	float2 uv = input.ShadowTex.xy / input.ShadowTex.w;
	
	//シャドウマップテクスチャの範囲内か？
	if (uv.x >= 0.0f && uv.x <= 1.0f) 
	{
		if (uv.y >= 0.0f && uv.y <= 1.0f) 
		{
			//シャドウマップテクスチャに格納されている遮蔽物までの距離を取得
			float4 shadowmapz = g_ShadowMap.Sample(g_SamplerLinear, uv);

			// 光源から当該ピクセルまでの距離を求める
			float lengthfromlight = input.LengthFromLight.z / input.LengthFromLight.w;

			// 影になるかを判断
			if (lengthfromlight - shadowmapz.r > 0.0f) 
			{
				//色を濃くする
				col.rgb *= 0.5f;
			}
		}
	}

	return outcol;
}