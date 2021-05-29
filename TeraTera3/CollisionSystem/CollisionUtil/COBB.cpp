
#include "COBB.h"
#include "../../WindowsSystem/DX11Settransform.h"

using namespace DirectX;

bool COBB::CompareLength(
    const OBB &boxA,
    const OBB &boxB,
    const XMFLOAT3 &vecSeparate,
    const XMFLOAT3 &vecDistance)
{
    float fDistance;

    DX11Vec3Dot(fDistance, vecDistance, vecSeparate);
    fDistance = fabsf(fDistance);

    float fShadowA = 0;

    float fShadowB = 0;

    float fShadowAx;
    float fShadowAy;
    float fShadowAz;

    DX11Vec3Dot(fShadowAx, vecSeparate, boxA.vecAxisX);
    fShadowAx = fabsf(fShadowAx * (boxA.fLengthX / 2));

    DX11Vec3Dot(fShadowAy, vecSeparate, boxA.vecAxisY);
    fShadowAy = fabsf(fShadowAy * (boxA.fLengthY / 2));

    DX11Vec3Dot(fShadowAz, vecSeparate, boxA.vecAxisZ);
    fShadowAz = fabsf(fShadowAz * (boxA.fLengthZ / 2));

    fShadowA = fShadowAx + fShadowAy + fShadowAz;

    float fShadowBx;
    float fShadowBy;
    float fShadowBz;

    DX11Vec3Dot(fShadowBx, vecSeparate, boxB.vecAxisX);
    fShadowBx = fabsf(fShadowBx * (boxB.fLengthX / 2));

    DX11Vec3Dot(fShadowBy, vecSeparate, boxB.vecAxisY);
    fShadowBy = fabsf(fShadowBy * (boxB.fLengthY / 2));

    DX11Vec3Dot(fShadowBz, vecSeparate, boxB.vecAxisZ);
    fShadowBz = fabsf(fShadowBz * (boxB.fLengthZ / 2));

    fShadowB = fShadowBx + fShadowBy + fShadowBz;

    if (fDistance > fShadowA + fShadowB)
    {
        return false;
    }

    return true;
}

void COBB::CalculateBoundingBox(DatFileLoader *datdata)
{

    int numvertices;

    float *x;
    float *y;
    float *z;

    float minx, miny, minz;
    float maxx, maxy, maxz;
    float cx, cy, cz;

    void *pVertices = NULL;

    numvertices = datdata->m_VertexSuu;

    // 頂点数分の座標格納エリア確保
    x = new float[numvertices];
    y = new float[numvertices];
    z = new float[numvertices];

    for (int i = 0; i < numvertices; i++)
    {
        x[i] = datdata->m_Vertex[i].m_Pos.x;
        y[i] = datdata->m_Vertex[i].m_Pos.y;
        z[i] = datdata->m_Vertex[i].m_Pos.z;
    }

    for (int i = 0; i < numvertices; i++)
    {
        if (i == 0)
        {
            minx = maxx = x[i];
            miny = maxy = y[i];
            minz = maxz = z[i];
        }
        if (minx >= x[i])
            minx = x[i];
        if (miny >= y[i])
            miny = y[i];
        if (minz >= z[i])
            minz = z[i];
        if (maxx <= x[i])
            maxx = x[i];
        if (maxy <= y[i])
            maxy = y[i];
        if (maxz <= z[i])
            maxz = z[i];
    }

    cx = (maxx + minx) / 2;
    cy = (maxy + miny) / 2;
    cz = (maxz + minz) / 2;

    float distance;
    float maxdistance = 0;

    for (int i = 0; i < numvertices; i++)
    {
        distance = (x[i] - cx) * (x[i] - cx) + (y[i] - cy) * (y[i] - cy) + (z[i] - cz) * (z[i] - cz);
        if (maxdistance < distance)
            maxdistance = distance;
    }

    maxdistance = sqrtf(maxdistance);
    delete[] z;
    delete[] y;
    delete[] x;

    m_obbinfo.currentcenter.x = cx;
    m_obbinfo.currentcenter.y = cy;
    m_obbinfo.currentcenter.z = cz;

    m_obbinfo.center.x = cx;
    m_obbinfo.center.y = cy;
    m_obbinfo.center.z = cz;

    m_obbinfo.fLengthX = fabsf(maxx - minx);
    m_obbinfo.fLengthY = fabsf(maxy - miny);
    m_obbinfo.fLengthZ = fabsf(maxz - minz);

    m_obbinfo.vecAxisX = XMFLOAT3(1.0f, 0.0f, 0.0f);
    m_obbinfo.vecAxisY = XMFLOAT3(0.0f, 1.0f, 0.0f);
    m_obbinfo.vecAxisZ = XMFLOAT3(0.0f, 0.0f, 1.0f);
}

void COBB::Update(DirectX::XMFLOAT4X4 mtxworld)
{
    m_obbinfo.vecAxisX = XMFLOAT3(mtxworld._11, mtxworld._12, mtxworld._13);
    m_obbinfo.vecAxisY = XMFLOAT3(mtxworld._21, mtxworld._22, mtxworld._23);
    m_obbinfo.vecAxisZ = XMFLOAT3(mtxworld._31, mtxworld._32, mtxworld._33);

    m_matworld = mtxworld;

    DX11Vec3MulMatrix(m_obbinfo.currentcenter, m_obbinfo.center, mtxworld);

    m_matworld._41 = m_obbinfo.currentcenter.x;
    m_matworld._42 = m_obbinfo.currentcenter.y;
    m_matworld._43 = m_obbinfo.currentcenter.z;
}

void COBB::Draw()
{
    DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_matworld);
}

bool COBB::Collision(COBB &obb)
{
    XMFLOAT3 vecSeparate;

    XMFLOAT3 vecDistance;
    XMFLOAT3 obbpos = obb.GetPosition();
    vecDistance.x = m_obbinfo.currentcenter.x - obbpos.x;
    vecDistance.y = m_obbinfo.currentcenter.y - obbpos.y;
    vecDistance.z = m_obbinfo.currentcenter.z - obbpos.z;

    OBB boxB, boxA;
    boxA = this->GetOBB();
    boxB = obb.GetOBB();

    bool sts;

    XMFLOAT3 *OBB_A_Axis[3] = {
        &boxA.vecAxisX,
        &boxA.vecAxisY,
        &boxA.vecAxisZ,
    };

    XMFLOAT3 *OBB_B_Axis[3] = {
        &boxB.vecAxisX,
        &boxB.vecAxisY,
        &boxB.vecAxisZ,
    };

    for (int i = 0; i < 3; i++)
    {
        vecSeparate.x = OBB_A_Axis[i]->x;
        vecSeparate.y = OBB_A_Axis[i]->y;
        vecSeparate.z = OBB_A_Axis[i]->z;

        DX11Vec3Normalize(vecSeparate, vecSeparate);

        sts = CompareLength(boxA,
                            boxB,
                            vecSeparate,
                            vecDistance);
        if (sts == false)
        {
            return false;
        }
    }

    // OBB-Bの３軸を分離軸にしてチェック
    for (int i = 0; i < 3; i++)
    {
        vecSeparate.x = OBB_B_Axis[i]->x;
        vecSeparate.y = OBB_B_Axis[i]->y;
        vecSeparate.z = OBB_B_Axis[i]->z;

        DX11Vec3Normalize(vecSeparate, vecSeparate);

        sts = CompareLength(boxA,
                            boxB,
                            vecSeparate,
                            vecDistance);

        if (sts == false)
        {
            return false;
        }
    }

    for (int p1 = 0; p1 < 3; p1++)
    {
        for (int p2 = 0; p2 < 3; p2++)
        {
            XMFLOAT3 crossseparate; // 外積ベクトル

            // 外積を計算する
            DX11Vec3Cross(crossseparate, *OBB_A_Axis[p1], *OBB_B_Axis[p2]);
            DX11Vec3Normalize(crossseparate, crossseparate); // 正規化

            sts = CompareLength(
                boxA,
                boxB,
                crossseparate,
                vecDistance);
            if (sts == false)
            {
                return false;
            }
        }
    }

    return true;
}