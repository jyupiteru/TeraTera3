/**
 * @file ComCamera.h
 * @author jupiter
 * @brief ComCameraが記述されたヘッダ
 * @date 2020-08-22
 */
#include <memory>
#include "../../../Core/ComponentBase.h"
#include "../../Behavior/ComTransform/ComTransform.h"

#include "../../../../System/CVector.h"
#include "../../../../WindowsSystem/Memory.h"
#include "../../../../WindowsSystem/DX11Settransform.h"
#include "../../../../System/CMatrix/CMatrix.h"

#pragma once

//!@brief		回転させる際に固定するもの
//!@details		E_TYPE_USERANGEがONじゃないといけない
enum class E_TYPE_FIXED
{
    //!@brief カメラを中心として回転(モデル回転なしのFPS)
    //!@details	ワールドのx,y軸の回転しかできない
    CAMERA,

    //!@brief 注視点を中心として回転(カメラのみ回転)
    //!@details	ワールドのx,y軸の回転しかできない
    LOOKAT,

    //!@brief 外部から座標等を受け取りカメラ中心で回るとき
    MODELCAMERA,

    //!@brief 外部から座標等を受け取り注視点中心で回るとき
    MODELLOOKAT,

    //!@brief 両方設定する
    SETBOTH
};
/**
 * @brief カメラ用のコンポーネント
 */
class ComCamera : public ComponentBase
{
public:
    /**
	 * @brief 	カメラの回転する方法の指定&オブジェクトの座標の適応の対象
	 * @details	固定したい対象はm_outMatrixに
	 */
    E_TYPE_FIXED m_typeFixed = E_TYPE_FIXED::LOOKAT;

    //!@brief		カメラの位置
    CVector3<float> m_cEyePos;

    //!@brief		注視点の位置
    CVector3<float> m_cLookPos;

    //!@brief		カメラのどっちが上か指定
    CVector3<float> m_cUp;

    /**
     * @brief 位置修正用の変数
     */
    CVector3<float> m_offsetPos;

    /**
	 * @brief 	カメラと注視点の距離
	 */
    CVector<int> m_range;

    /**
     * @brief カメラの初期角度
     */
    CVector3<float> m_firstAngle;

    /**
     * @brief 外部から取り込みたい座標を含むもの
     */
    ComTransform *m_otherTransform = nullptr;

protected:
    //!@brief		カメラ(eye)の行列
    DirectX::XMFLOAT4X4 m_view;

    //!@brief		カメラ(eye)の行列
    DirectX::XMFLOAT4X4 m_projection;

    /**
	 * @brief 	一番近く
	 * @details	m_farとの間を表示する
	 */
    float m_near = 0.0f;

    /**
	 * @brief 	表示できる一番遠く
	 * @details	m_nearとの間を表示する
	 */
    float m_far = 0.0f;

    /**
	 * @brief 画面のアスペクト比
	 */
    float m_aspect = 0.0f;

    /**
	 * @brief 視野
	 */
    float m_fov = 0.0f;

public:
    ComCamera()
    {
        //初期化
        DX11MtxIdentity(m_view);
        DX11MtxIdentity(m_projection);
    };

    virtual ~ComCamera(){};

    virtual void Init() override;

    virtual void Ready() override;

    virtual void Update() override;

    virtual void ImGuiDraw(unsigned int windowid) override;

    //Set系
    /**
	 * @brief 			カメラの各メンバをセットする
	 * @param eye 		カメラの座標
	 * @param lookat 	注視点の座標
	 * @param up 		カメラのyベクトル
	 */
    void SetCamera(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 lookat, DirectX::XMFLOAT3 up)
    {
        m_cEyePos.SetValue(eye.x, eye.y, eye.z);
        m_cLookPos.SetValue(lookat.x, lookat.y, lookat.z);
        m_cUp.SetValue(up.x, up.y, up.z);
    }

    /**
	 * @brief 			プロジェクション行列を作成するためのメンバ
	 * @param nearclip 	表示する一番近くの座標
	 * @param farclip 	表示する一番遠くの座標
	 * @param fov 		視野
	 * @param width 	画面の横
	 * @param height 	画面の縦
	 */
    void SetProjection(float nearclip, float farclip, float fov, float width, float height)
    {
        m_near = nearclip;
        m_far = farclip;
        m_fov = fov;
        m_aspect = width / height;
        CreateProjectionMatrix();
    }

    /**
     * @brief ビュー行列を取得する関数
     * @return DirectX::XMFLOAT4X4 取得したいビュー行列
     */
    [[nodiscard]] DirectX::XMFLOAT4X4 GetEyeMatrix()
    {
        return m_view;
    }

    /**
     * @brief プロジェクション行列を取得するメソッド
     * @return DirectX::XMFLOAT4X4 取得したいプロジェクション行列
     */
    [[nodiscard]] DirectX::XMFLOAT4X4 GetProjectionMatrix()
    {
        return m_projection;
    }

    /**
     * @brief フラスタムカリング用のtagPLANEを取得するメソッド
     * @param plane tagPLANEを格納する変数 return だといちど再生成してしまうので
     */
    void GetPLANEForFrustum(tagPLANE *plane);

    /**
     * @brief フラスタム(カメラの視界)に対象のオブジェクトがいるか確認するメソッド
     * @param obj 確認したい対象のオブジェクト
     * @param plane フラスタムの各平面の色々
     * @return true 存在する
     * @return false 存在しない
     * @details ここconstつけても本体から取得する際に生成したものなので問題なし
     */
    [[nodiscard]] bool IsInFrustum(const GameObject &obj, tagPLANE *plane)
    {

        auto [pos_x, pos_y, pos_z] = obj.m_transform->m_worldPosition.GetValue();
        auto [size_x, size_y, size_z] = obj.m_transform->m_size.GetValue();

        //各平面との距離を格納する変数
        float ans;

        for (int i = 0; i < 6; i++)
        {
            ans = plane[i].a * pos_x + plane[i].b * pos_y + plane[i].c * pos_z + plane[i].d;
            if (ans + size_x < 0 || ans + size_y < 0 || ans + size_z < 0)
            { //大きさを考え確認
                return false;
            }
        }

        return true;
    }

protected:
    //行列作成系
    /**
	 * @brief カメラ用の行列を作成しセットする
	 */
    void CreateViewMatrix();

    /**
	 * @brief プロジェクション行列を作成しセットする
	 */
    void CreateProjectionMatrix();

    /**
	 * @brief 		角度をラジアンに変換する
	 * @param num	変換したい角度をいれる
	 * @return 		float 変換されたラジアンが出る
	 */
    [[nodiscard]] float ToRad(float num)
    {
        return num * DirectX::XM_PI / 180;
    }

    /**
     * @brief
     * @param radius 		半径
     * @param elevation 	仰角	x軸
     * @param azimuth 		方位角	y軸
     * @return 				生成された座標が返される
     */
    [[nodiscard]] std::tuple<float, float, float> CreatePos(float radius, float elevation, float azimuth);
};