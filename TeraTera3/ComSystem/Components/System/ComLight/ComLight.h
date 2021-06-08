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

    /**
     * @brief 光のタイプを設定
     */
    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    /**
     * @brief シェーダに定数を渡すのに必要なバッファ
     */
    ID3D11Buffer *m_pConstantBufferLight = nullptr;

    ComCamera *m_pComCamera = nullptr;

public:
    /**
     * @brief 光のタイプを決めるのに使用
     */
    LightType m_type;

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