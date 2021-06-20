/**
 * @file ComLightPoint.h
 * @author jupiter ()
 * @brief ComLightPointクラスの宣言が記載されたヘッダ
 */

#include "../../../Core/ComponentBase.h"
#include "../../../../System/CVector.h"

#pragma once

class ComLight;

/**
 * @brief ポイントライトを操るコンポーネントクラス
 * @n 持たせたオブジェクトの座標そのまま使用
 * @n まだ1つ下作成できないのでシングルトン化させる
 */
class ComLightPoint : public ComponentBase
{
    static ComLightPoint *m_instance;

public:
    friend ComLight;

    /**
     * @brief ポイントライトの色
     */
    CVector3<float> m_lightColor;

    /**
     * @brief 光が影響を与える範囲
     * @n 初期 100
     */
    CVector<float> m_range;

public:
    ComLightPoint();
    ~ComLightPoint();

    void Init() override;
    void Uninit() override;

protected:
    /**
     * @brief このポイントライトの実体を取得する処理 ComLightのみしか使用させないのでこれでいい
     * @return ComLightPoint& 取得したいこのクラスの実体
     */
    [[nodiscard]] static ComLightPoint *GetInstance();
};