/**
 * @file ComCameraCustom.h
 * @author jupiter ()
 * @brief ComCameraCustomの宣言が書かれたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief カメラをカスタムしたコンポーネント
 */
class ComCameraCustom : public ComponentBase
{
public:
    ComCameraCustom(){};
    ~ComCameraCustom(){};

    /**
     * @brief プレイヤーの座標取得用のコンポーネントのポインタ
     */
    ComTransform *m_playerTransform = nullptr;

    /**
     * @brief カメラコンポーネントへのポインタ
     */
    ComCamera *m_camera = nullptr;

    void Init(){};

    void Uninit(){};

    void Ready();

    void Update();

    void Draw(){};
};
