
////
//	フォングの鏡面反射のピクセルシェーダー
// 
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

	float d = dot(L, N);		//法線と光の方向を内積して0と比較 大きいほうを格納
	d = max (0.0,d);            //0とdの大きいほうを格納する

	float4 diffuse = d * Ambient;	//環境光がどれくらい影響があるか計算
	//diffuse *=  diffuseMaterial;	//??

    //反射ベクトルの計算    
    float3 reflectVec = reflect(diffuse,N);

    //反射面から視点に伸びるベクトルの計算して正規化
    float toEyeVec = EyePos - input.Pos;
    toEyeVec = normalize(toEyeVec);
    
    //反射面から反射したベクトルを視点までのベクトルに落としその大きさを求める
    float t = dot(reflectVec, toEyeVec);

    if(t < 0.0f)
    {
        t=0.0f;
    }
    
    //鏡面販社の強さを絞る(2個目の引数)
    t = pow( t, 50.0f);
    
    //鏡面反射光を求める
    float specularllig = Ambient * t;// * specularMaterial;
    
    float4 lig = diffuse + specularllig;
    
	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;
	
	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);	//マテリアル色?の取得
	col *=  texcol;		
    
    //マテリアル色を環境光とかける
    col *= lig;
    
    
	col.a = 1.0f;	//?
    
	return col;
}