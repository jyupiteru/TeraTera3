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
        /**
         * @brief 正規化していない光のさす方向の情報
         */
        DirectX::XMFLOAT3 LightDirection;

        float pad; //穴埋め用変数

        /**
         * @brief カメラの座標情報？
         */
        DirectX::XMFLOAT3 EyePos;

        float pad2; //穴埋め用変数２

        /**
         * @brief ライトの環境光情報
         */
        DirectX::XMFLOAT4 Ambient;
    };

    /*ALIGN16 struct tag
    {

    };*/

    /**
     * @brief ライトの種類
     */
    enum class E_TYPE_LIGHT
    {
        /**
         * @brief ライトの方向とカラーのみを持つライト
         * @n 位置情報がないので移動しても光の強さ、方向は変わらない = めちゃくちゃ遠くからあたっているイメージ 影変えるには方向を変える必要あり
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
    E_TYPE_LIGHT m_typeLight;

    /**
     * @brief 色の方向情報
     * @n どの角度からあたるかなどを入れる めんどくさかったらライトの座標入れてもいいかも(正規化するので)
     */
    CVector3<float> m_lightDirection;

    /**
     * @brief 環境光の色
     * @n 各種モデル等に反映される
     */
    CVector3<float> m_lightColor;

public:
    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Ready() override;
    virtual void Update() override;
    virtual void ImGuiDraw(unsigned int windowid) override;

private:
    /**
     * @brief ライトの更新処理
     */
    void UpdateLight();

    /**
     * @brief ディレクションライトの更新処理
     */
    void UpdateDirectionLight();
};