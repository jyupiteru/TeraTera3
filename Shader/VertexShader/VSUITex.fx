#include	"../Utils/CommonPSVS.fx"

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(	float4 Pos		: POSITION,
				float4 Color	: COLOR,
				float2 Tex		: TEXCOORD)
{

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);			// ワールド変換

	// スクリーン座標に変換 Setup.hの画面縦横を入れる
    output.Pos.x = ((output.Pos.x / ViewportWidth.x)  - 0.5f);
    output.Pos.y = (0.5f - (output.Pos.y / ViewportHeight.x));
	output.Pos.z = 0.0f;

	output.Color = Color;					// カラー

	// テクスチャ座標
	output.Tex = Tex;

	return output;
}
