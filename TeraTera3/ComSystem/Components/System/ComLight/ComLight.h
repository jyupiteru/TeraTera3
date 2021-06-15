/**
 * @file ComLight.h
 * @author jupiter
 * @brief ComLightを記述したヘッダ
 * @version 1.0
 */
#include <DirectXMath.h>

#include "../../../Core/ComponentBase.h"
#include "../ComCamera/ComCamera.h"

#pragma once

class ComSphere;

/**
 * @brief ゲーム内の光を管理するコンポーネント
 */
class ComLight : public ComponentBase
{
    /**
     * @brief シェーダに渡す情報
     */
    ALIGN16 struct ConstantBufferLight
    {
        DirectX::XMFLOAT4 LightDirection;
        DirectX::XMFLOAT4 EyePos;
        DirectX::XMFLOAT4 Ambient;
    };

    /*ALIGN16 struct tag
    {

    };*/

    /**
     * @brief ライトの種類
     */
    enum class E_LightType
    {
        /**
         * @brief ライトの方向とカラーのみを持つライト
         * @n 位置情報がないので移動しても光の強さ、方向は変わらない
         */
        DIRECTION,

        /**
         * @brief ライトの位置、色、影響範囲(メートル)を持つ
         * @n DIRECTIONより少し重いらしい
         */
        POINT,

        /**
         * @brief ライトの位置、色、影響範囲、放射方向、放射角度、影響範囲(メートル)
         */
        SPOT
    };

    /**
     * @brief シェーダに定数を渡すのに必要なバッファ
     */
    ID3D11Buffer *m_pConstantBufferLight = nullptr;

    /**
     * @brief 各種保持しているコンポーネントへのアクセス簡易用変数
     */
    ComCamera *m_pComCamera = nullptr;
    ComSphere *m_pComSphere = nullptr;

public:
    /**
     * @brief 光のタイプを決めるのに使用
     */
    E_LightType m_type;

    /**
     * @brief ？？？
     */
    CVector4<float> m_ambient;

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Ready() override;

    virtual void Update() override;

    virtual void ImGuiDraw(unsigned int windowid) override;
};