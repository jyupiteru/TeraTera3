////
//	2D表示の単方向ワイプを行うシェーダー
//	定数バッファの設定(ワイプ用)が必要
////

#include "../Utils/CommonPSVS.fx"


float4 main(VS_OUTPUT input) : SV_Target
{
	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	//ワイプを行う方向を正規化する
	float2 wipevec = wipeVector;
	wipevec = normalize(wipevec);

	//正規化済みのワイプの方向と座標を内積する
	float t = dot(wipevec,input.Pos.xy);

	float flag = wipeFlag;
	flag = normalize(flag);

	//座標から現在の表示をさせない範囲(ワイプ)を引き0未満になったら表示をやめる
	clip((t - wipeSize) * flag) ;

	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	//これで透過する
	texcol *= col;

	return texcol;
}