/**
 * @file ComLight.h
 * @author jupiter
 * @brief ComLightを記述したヘッダ
 * @version 1.0
 */
#include <DirectXMath.h>
#include <d3d11.h>

#include "../../../Core/ComponentBase.h"
#include "../../../../System/Dx11mathutil/Dx11mathutil.h"

#pragma once

class ComSphere;
class ComCamera;

/**
 * @brief ディレクショナルライトを使用するのに必要な情報を集めた構造体
 */
ALIGN16 struct tagDirectionalLight
{
     /**
         * @brief 正規化済みのディレクションライトの方向ベクトル
         */
     DirectX::XMFLOAT3 directionalLightDirection;

     float pad; //穴埋め用変数

     /**
      * @brief ディレクションライトの光情報
      * @n 0.0~1.0に変換済み
      */
     DirectX::XMFLOAT4 directionalLightColor;
};

/**
 * @brief ポイントライトを使用するのに必要な情報を集めた構造体
 */
ALIGN16 struct tagPointlight
{
     /**
      * @brief ポイントライトの座標
      */
     DirectX::XMFLOAT4 pointLightPosition;

     /**
      * @brief ポイントライトの色
      * @n 0.0~1.0に変換済み
      */
     DirectX::XMFLOAT4 pointLightColor;

     /**
      * @brief ポイントライトの影響範囲
      */
     float pointLightRange;

     DirectX::XMFLOAT3 pad;
};

/**
* @brief ライトのシェーダに渡す情報
*/
ALIGN16 struct tagConstantBufferLight
{
     /**
      * @brief ディレクションライトの情報
      */
     tagDirectionalLight directionalLight;

     /**
      * @brief ポイントライトの情報
      */
     tagPointlight pointLight;

     //各ライト共通項?

     /**
         * @brief カメラの座標情報？
         */
     DirectX::XMFLOAT3 eyePos;

     float pad3; //穴埋め用変数２

     /**
      * @brief 環境光情報
      * @n 0.0~1.0に変換済み
      */
     DirectX::XMFLOAT4 ambient;
};

/**
 * @brief ゲーム内の光を管理するコンポーネント
 * @n オブジェクトの座標をもとに光の方向を管理します
 */
class ComLight : public ComponentBase
{

     //  ディレクションライト
     //  ライトの方向とカラーのみを持つライト
     //  位置情報がないので移動しても光の強さ、方向は変わらない = めちゃくちゃ遠くからあたっているイメージ 影変えるには方向を変える必要あり

     //  ポイントライト
     //  ライトの位置、色、影響範囲(メートル)を持つ
     //  DIRECTIONより少し重いらしい

     //  スポットライト
     //  ライトの位置、色、影響範囲、放射方向、放射角度、影響範囲(メートル)

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
     * @brief ディレクションライトの色
     * @n 各種モデル等に反映される
     */
     CVector3<float> m_directionalColor;

     /**
     * @brief 環境光の色
     * @n 各種モデル等に反映される
     */
     CVector3<float> m_ambientColor;

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
};