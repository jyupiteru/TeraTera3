


//ランバートの拡散反射を計算して返す処理
//正規化処理は関数内で行っている
float4 CalcLambertDiffuse(float4 _lightdirection, float4 _lightcolor, float4 _normal)
{

    //光の方向ベクトルの必要でない値を0,0fにして正規化
    _lightdirection.w = 0.0f;
    _lightdirection = normalize(_lightdirection);

    //法線の必要でない値を0,0fにして正規化
    _normal.w = 0.0f;
    _normal = normalize(_normal);

    //法線と光の方向を内積
    float d = dot(_normal, _lightdirection);

    //内積は同じ方向なら+になり逆ならば-になる
    //法線が光の方向にどれくらい向いているのか知りたいので-1をかける
    d *= -1;

    //0.0fと比較して大きいほうを返す(0以下はつまり光が当たっていないので)
    d = max(0.0f ,d);

    //環境光を計算し返す
    return _lightcolor * d;
}


//鏡面反射光を求める関数
float4 CalcPhongSpecular(float4 _lightdirection, float4 _lightcolor, float4 _worldpos, float4 _normal, float4 _eyepos, float _pow)
{
    _eyepos.w = 0.0f;
    _worldpos.w = 0.0f;
    
    //光の方向ベクトルの必要でない値を0,0fにして正規化
    _lightdirection.w = 0.0f;
    _lightdirection = normalize(_lightdirection);

    //法線の必要でない値を0,0fにして正規化
    _normal.w = 0.0f;
    _normal = normalize(_normal);

    // 反射ベクトル(当たって反射したライトのベクトル)を求める
    float4 reflectVec = reflect(_lightdirection, _normal);

	//カメラ座標 - ポリゴンの座標 で当たった面からカメラまでのベクトルを求める
	float4 toEyeVec = _eyepos  - _worldpos;
	toEyeVec = normalize(toEyeVec);

	//どれくらい目に入ってくるかを計算 0以下は0に設定
	float t = dot(reflectVec,toEyeVec);
	t = max(0.0, t);

	//累乗を行い鏡面反射の強さを絞る ハイライトの強さをここで絞る
	t = pow(t,_pow);

    // 鏡面反射光を求める
    return _lightcolor * t;
}