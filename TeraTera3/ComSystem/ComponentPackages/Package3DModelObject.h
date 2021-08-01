/**
 * @file Package3DModelObject.h
 * @author jupiter
 * @brief Package4DModelを記述したヘッダ
 */
#include "../Core/PackageBase.h"

#include "../Components/Behavior/Com3DModel/Com3DModel.h"

#pragma once

class Package3DModelObject : public PackageBase
{
public:
    /**
     * @brief  Package 3 D Model Objectの生成時処理
     */
    Package3DModelObject(){};

    /**
     * @brief Package 3 D Model Objectの破棄時処理
     */
    ~Package3DModelObject(){};

    /**
     * @brief 初期処理
     */
    void Init() override
    {
        AddComponent<ComShader>();
        AddComponent<Com3DModel>();
    }
};
