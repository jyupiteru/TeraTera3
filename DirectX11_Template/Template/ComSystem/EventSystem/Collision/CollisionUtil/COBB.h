
#pragma once
#include <DirectXMath.h>
#include "../../../../Systems/CDirectXGraphics/CDirectXGraphics.h"
#include "../../../../Systems/Dx11mathutil/Dx11mathutil.h"
#include "../../../../Systems/Model/DatFileLoader/DatFileLoader.h"

class DatFileLoader;

// OBB用構造体
struct OBB
{
    DirectX::XMFLOAT3 vecAxisX;
    DirectX::XMFLOAT3 vecAxisY;
    DirectX::XMFLOAT3 vecAxisZ;
    DirectX::XMFLOAT3 center;
    DirectX::XMFLOAT3 currentcenter;
    float fLengthX;
    float fLengthY;
    float fLengthZ;
};

class COBB
{
private:
    OBB m_obbinfo;
    DirectX::XMFLOAT4X4 m_matworld;

    bool CompareLength(
        const OBB &pboxA,
        const OBB &pboxB,
        const DirectX::XMFLOAT3 &pvecSeparate,  // 分離軸
        const DirectX::XMFLOAT3 &pvecDistance); // ２つのオブジェクトを結ぶベクトル

    void CreateBoxMesh()
    {
        /*m_boxmesh.Init(
            m_obbinfo.fLengthX,
            m_obbinfo.fLengthY,
            m_obbinfo.fLengthZ);*/
    }

    void CalculateBoundingBox(DatFileLoader *datdata);

public:
    COBB()
    {
        DX11MtxIdentity(m_matworld);
    }

    ~COBB()
    {
        Exit();
    }

    void Init(DatFileLoader *datdata)
    {
        CalculateBoundingBox(datdata);
        CreateBoxMesh();
    }

    void Exit()
    {
    }

    void Update(DirectX::XMFLOAT4X4 matworld);

    void Draw();

    bool Collision(COBB &obb);

    DirectX::XMFLOAT3 GetPosition()
    {
        return m_obbinfo.currentcenter;
    }

    OBB GetOBB()
    {
        return m_obbinfo;
    }
};