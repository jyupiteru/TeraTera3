
////
//	フォングの鏡面反射のピクセルシェーダー
//	法線必須
////


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

	//法線と光の方向を内積して0と比較 大きいほうを格納
	float d = dot(N, L);

	//内積してマイナスになったのが光の方向を向いているので-1をかけて0以下を0にする
	d *= -1.0f;
	d = max(0.0f,d);

	//環境光がどれくらい影響があるか計算
	float4 diffuseLig = d * Ambient;
	
	//反射ベクトル(当たって反射したライトのベクトル)を求める
	float4 reflectVec = reflect(L, N);

	//カメラ座標 - ポリゴンの座標 で当たった面からカメラまでのベクトルを求める
	float4 toEyeVec = EyePos - input.WPos;
	toEyeVec = normalize(toEyeVec);

	//どれくらい目に入ってくるかを計算 0以下は0に設定
	float t = dot(reflectVec,toEyeVec);
	t = max(0.0, t);

	//累乗を行い鏡面反射の強さを絞る ハイライトの強さをここで絞る
	t = pow(t,5.0f);

	//鏡面反射光を求める
	float specularLig = Ambient * t;

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

	return outcol;
}