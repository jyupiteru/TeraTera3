////
//シャドウマップを制作するのに使用するピクセルシェーダ
////

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos	:	POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, viewFromLight);
	output.Pos = mul(output.Pos, projectionFromLight);

	output.Depth = output.Pos;

	return output;
}
