/**
 * @file Package3DTexObject.h
 * @author jupiter
 * @brief 画像表示用のパッケージを記述したヘッダ
 * @date 2020-08-22
 */
#include "../Core/PackageBase.h"

#include "../Components/Behavior/Com3DTexture/Com3DTexture.h"

#pragma once

/**
 * @brief 画像表示等のコンポーネントを集めたパッケージ
 */
class Package3DTexObject : public PackageBase
{
public:
    /**
     * @brief  Package 3 D Tex Objectの生成時処理
     */
    Package3DTexObject(){};

    /**
     * @brief Package 3 D Tex Objectの破棄時処理
     */
    ~Package3DTexObject(){};

    /**
     * @brief 初期処理
     */
    void Init() override
    {
        AddComponent<ComShader>();
        AddComponent<Com3DTexture>();
    }
};
