﻿/**
 * @file ComShot.h
 * @author jupiter ()
 * @brief ComShotクラスの宣言が記述されたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief 弾にセットするコンポーネント
 */
class ComShot : public ComponentBase
{
public:
    /**
     * @brief 弾の速度をセットする変数
     */
    CVector3<float> m_shotSpeed;

public:
    ComShot(){};
    ~ComShot(){};
    void Update() override;
};
