/**
 * @file ComTransform.h
 * @author jupiter ()
 * @brief Transformクラスを記述したヘッダ
 */

#include "../../../Core/ComponentBase.h"

#include "../../../../System/CVector.h"
#include "../../../../WindowsSystem/Dx11mathutil/Dx11mathutil.h"

#include <memory>
#pragma once

/**
 * @brief オブジェクトの基本的な座標等を管理するコンポーネントクラス
 * @n デフォルトで追加されます
 * @note Unityを参考に制作
 * @n 今思うとコンポーネントに分けると色々面倒だったきも？
 */
class ComTransform final : public ComponentBase
{
public:
    friend class GameObject;

    /**
     * @brief 現在のワールド(シーン)上の座標
     * @n モデルなら初期位置はモデルの中心になります
     * @details 移動するなら差分をvectorに足すことを推奨
     * @n 0,0,0 + local = world
    */
    CVector3<float> m_worldPosition;

    /**
     * @brief 現在のローカル座標
     * @n 0,0,0を基準にどの位置に初期位置(中心座標)があるか記録
     */
    CVector3<float> m_localPosition;

    /**
     * @brief 前のフレームからの移動分
     * @todo 移動量の追加にy軸を固定にするかどうかの処理を追加する？
     */
    CVector3<float> m_vector;

    /**
     * @brief 角度
     */
    CVector3<float> m_angle;

    /**
     * @brief 大きさ
     */
    CVector3<float> m_size;

    /**
     * @brief 色情報 RGBA
     * @n 256,256,256,1.0がそれぞれの最大値
     */
    CVector4<float> m_color;

    /**
     * @brief 読み込んだモデル等の大きさとこのワールドの1単位の大きさとの比率
     * @details 基本使用しない（モデルで使用します）
     */
    CVector<float> m_sizeRateSize;

    /**
     * @brief モデル等の初期サイズを格納
     */
    CVector3<float> m_offsetSize;

private:
    /**
     * @brief 前回回転した量
     */
    DirectX::XMFLOAT3 m_lastAngle = {0, 0, 0};

    /**
     * @brief 前回の大きさ
     */
    DirectX::XMFLOAT3 m_lastSize = {1, 1, 1};

    //!@brief 行列
    DirectX::XMFLOAT4X4 m_matrix;

    //!@brief 拡大処理を行っていない行列
    DirectX::XMFLOAT4X4 m_noScalingMatrix;

public:
    ComTransform()
    {
        Init();
    }

    ~ComTransform(){};

    /**
     * @brief 初期処理
     */
    virtual void Init() override;

    /**
     * @brief 破棄時処理
     */
    virtual void Uninit() override;

    //!@brief		毎フレーム動く処理突っ込む！
    virtual void Update() override
    {
        UpdateMatrix();
    }

    virtual void ImGuiDraw(unsigned int windowid) override;

    /**
	 * @brief 			行列のみをセットするクラス
	 * @details			SetPosで座標をセットする必要あり
	 * @param matrix 	セットしたい行列を入れる
	 */
    void SetMatrix(const DirectX::XMFLOAT4X4 &matrix)
    {
        m_matrix = matrix;
    }

    /**
	 * @brief						行列を取得する関数
	 * @return DirectX::XMFLOAT4X4 	取得したい行列
     * @details 初期角度追加後の座標なので注意
	 */
    [[nodiscard]] const DirectX::XMFLOAT4X4 &GetMatrix()
    {
        return m_matrix;
    }

    [[nodiscard]] const DirectX::XMFLOAT4X4 &GetNoScalingMatrix()
    {
        return m_noScalingMatrix;
    }

private:
    /**
     * @brief 行列関連の毎フレーム行う処理
     */
    void UpdateMatrix();
};
