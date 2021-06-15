


#include	"../commonpsvs.fx"

float4 main(VS_OUTPUT input) : SV_Target
{
	float4 N = input.Normal;    //法線取り出し
	float4 L = LightDirection;	//光の方向取り出し
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。
	L.w = 0.0f;					// 光の方向はベクトルなのでＷの値を０にする。

	//正規化をする
	N = normalize(N);
	L = normalize(L);

	float d = dot(L, N);		//法線と光の方向を内積して0と比較 大きいほうを格納
	d = max (0.0,d);            //0とdの大きいほうを格納する

	float4 diffuse = diffuseMaterial * d * Ambient;

	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
	float4 col = Ambient + diffuse * texcol;
	col.a = 1.0f;
	return col;
}