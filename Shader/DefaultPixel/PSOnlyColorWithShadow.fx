
#include	"../Utils/CommonPSVS.fx"

float4 main(VS_OUTPUT input) : SV_Target
{
	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	// シャドウマップから遮蔽物までの距離を取り出す
	float2 uv = input.ShadowTex.xy / input.ShadowTex.w;

	//シャドウマップテクスチャの範囲内か？
	if (uv.x > 0.0f && uv.x < 1.0f && 
			uv.y > 0.0f && uv.y < 1.0f) 
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

	return col;
}