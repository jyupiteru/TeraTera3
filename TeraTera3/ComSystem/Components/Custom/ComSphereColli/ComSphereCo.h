//
//
//#include <d3d11.h>
//#include <memory>
//#include "../../../Core/Cores.h"
//
//#include "../../../../Systems/Model/DatFIle/DatFile.h"
//#include "../../../../Systems/Model/CSphere/CSphere.h"
//#include "../../../../Systems/DX11Settransform.h"
//
//#pragma once
//struct HITDATA
//{
//    float minx, miny, minz; //バウンディングボックス最小値
//    float maxx, maxy, maxz; //バウンディングボックス最大値
//    float cx, cy, cz;       //バウンディングボックス中心座標
//    float r;                //バウンディングボックス半径
//};
//
//class ComponentSphere : public ComponentBase
//{
//private:
//    HITDATA m_hitData;            //当たり判定用のデータ
//    DirectX::XMFLOAT3 m_wpos;     //ワールド空間上の位置情報
//    DirectX::XMFLOAT4X4 m_matrix; //境界球の描画用行列
//    X::Material m_material;       //マテリアル
//    CSphere m_shperemesh;         //バウンディングスフィア半径
//
//public:
//    ComponentSphere(){};
//    ~ComponentSphere()
//    {
//        Uninit();
//    }
//
//    void Init() override;
//
//    void Uninit() override
//    {
//        m_shperemesh.Exit();
//    }
//
//    void Update() override;
//
//    void Draw() override;
//
//    float GetR() const
//    {
//        return m_hitData.r;
//    }
//
//    const DirectX::XMFLOAT3 &GetWpos()
//    {
//        return m_wpos;
//    }
//
//    const HITDATA &GetHitdata()
//    {
//        return m_hitData;
//    }
//
//private:
//    void Traneparentmaterial(float alpha)
//    {
//        m_material.m_Diffuse = {1.0f, 1.0f, 1.0f};
//        m_material.m_Ambient = {1.0f, 1.0f, 1.0f};
//        m_material.m_Specular = {1.0f, 1.0f, 1.0f};
//    }
//
//    void CalculateBoundingSphere(DatFileLoader *datdate);
//
//    void CreateSphereMesh(float r)
//    {
//        bool sts = m_shperemesh.Init(r,                //半径
//                                     50,               //水平方向の分割数
//                                     50,               //垂直方向の分割数
//                                     GetDX11Device()); //デバイスオブジェクト
//        if (!sts)
//        {
//            MessageBox(nullptr, TEXT("CSphere Init Error"), TEXT("error"), MB_OK);
//        }
//    }
//};
