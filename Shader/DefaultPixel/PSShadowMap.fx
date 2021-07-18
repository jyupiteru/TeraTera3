////
//シャドウマップを制作するのに使用するピクセルシェーダ
////

#include "../Utils/CommonPSVS.fx"

float4 main(VS_OUTPUT input) : SV_Target
{
	float depth = input.Depth.z / input.Depth.w;
	return depth;
}
