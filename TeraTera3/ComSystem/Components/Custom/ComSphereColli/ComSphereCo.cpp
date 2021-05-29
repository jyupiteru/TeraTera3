//
//#include "ComSphereCo.h"
//#include "../../DefaultComponents.h"
//
//const float PI = DirectX::XM_PI;
//
//void ComponentSphere::Init()
//{
//    Traneparentmaterial(0.3f); //透明度の設定
//    CalculateBoundingSphere(m_gameObject->GetComponent<Com3DModel>()->GetModelData());
//    CreateSphereMesh(m_hitData.r);
//}
//
//void ComponentSphere::Update()
//{
//    DirectX::XMFLOAT3 pos = {m_hitData.cx, m_hitData.cy, m_hitData.cz};
//
//    //行列*ベクトル（ワールド座標に変換）
//    DX11Vec3MulMatrix(m_wpos, pos, m_gameObject->m_transform->GetMatrix());
//
//    m_matrix = m_gameObject->m_transform->GetMatrix();
//    m_matrix._41 = m_wpos.x;
//    m_matrix._42 = m_wpos.y;
//    m_matrix._43 = m_wpos.z;
//}
//
//void ComponentSphere::Draw()
//{
//    ID3D11DeviceContext *device;
//    device = GetDX11DeviceContext();
//    //ワールド座標返還行列
//    DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_matrix);
//    m_shperemesh.Draw(device);
//}
//
//void ComponentSphere::CalculateBoundingSphere(DatFileLoader *datdate)
//{
//    HITDATA atari;   //あたり判定データ格納構造体
//    int numvertices; //頂点数
//
//    float *x; //頂点座標データ
//    float *y; //頂点座標データ
//    float *z; //頂点座標データ
//
//    float minx, miny, minz; //最小値(X,Y,Z)
//    float maxx, maxy, maxz; //最大値(X,Y,Z)
//    float cx, cy, cz;       //中心座標
//
//    void *pVertices = nullptr;
//
//    numvertices = datdate->m_VertexSuu; //頂点数を取得
//
//    //頂点数分の座標格納エリアを確保
//    x = new float[numvertices];
//    y = new float[numvertices];
//    z = new float[numvertices];
//
//    int i = 0;
//
//    //頂点をすべて取り出す
//    while (i < numvertices)
//    {
//        x[i] = datdate->m_Vertex[i].m_Pos.x; //X座標取得
//        y[i] = datdate->m_Vertex[i].m_Pos.y; //Y座標取得
//        z[i] = datdate->m_Vertex[i].m_Pos.z; //X座標取得
//        i++;
//    }
//
//    i = 0;
//    //XYZそれぞれについて、最大値、最小値を求める
//    while (i < numvertices)
//    {
//        if (i == 0)
//        {
//            minx = maxx = x[i];
//            miny = maxy = y[i];
//            minz = maxz = z[i];
//        }
//        if (minx >= x[i])
//        {
//            minx = x[i];
//        }
//        if (miny >= y[i])
//        {
//            miny = y[i];
//        }
//        if (minz >= z[i])
//        {
//            minz = z[i];
//        }
//        if (maxx <= x[i])
//        {
//            maxx = x[i];
//        }
//        if (maxy <= y[i])
//        {
//            maxy = y[i];
//        }
//        if (maxz <= z[i])
//        {
//            maxz = z[i];
//        }
//        i++;
//    }
//
//    cx = (maxx + minx) / 2; //球の中心座標を求める
//    cy = (maxy + miny) / 2; //球の中心座標を求める
//    cz = (maxz + minz) / 2; //球の中心座標を求める
//
//    i = 0;
//    float distance;
//    float maxdistance = 0;
//
//    //全頂点と中心座標との距離を求める
//    while (i < numvertices)
//    {
//        //2点間の距離を求める
//        distance = (x[i] - cx) * (x[i] - cx) + (y[i] - cy) * (y[i] - cy) + (z[i] - cz) * (z[i] - cz);
//        if (distance > maxdistance)
//        {
//            maxdistance = distance;
//        }
//        i++;
//    }
//    //最大半径を計算
//    maxdistance = sqrtf(maxdistance);
//    delete[] z;
//    delete[] y;
//    delete[] x;
//
//    atari.minx = minx; //バウンディングボックスのXの最小値
//    atari.minz = minz; //バウンディングボックスのYの最小値
//    atari.maxx = maxx; //バウンディングボックスのXの最大値
//    atari.maxy = maxy; //バウンディングボックスのYの最大値
//    atari.maxz = maxz; //バウンディングボックスのZの最大値
//    atari.cx = cx;     //境界球の中心をセット
//    atari.cy = cy;     //境界球の中心をセット
//    atari.cz = cz;     //境界球の中心をセット
//
//    atari.r = maxdistance; //境界球の半径をセット
//
//    m_hitData = atari;
//}