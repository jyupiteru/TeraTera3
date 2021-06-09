/**
 * @file ComDataManager.h
 * @author jupiter ()
 * @brief ComDataManagerクラスの宣言が記述されたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief リザルトで使用する各種情報を保持するクラス
 */
class ComDataManager : public ComponentBase
{
    static ComDataManager *m_instance;

public:
    /**
     * @brief 現在の経過秒数
     */
    CVector<float> m_nowCount;

    /**
     * @brief 最大の秒数
     */
    CVector<float> m_maxTime;

public:
    ComDataManager() {};
    ~ComDataManager() {};
    void Init() override;
    void Uninit() override;

    [[nodiscard]] static ComDataManager &GetInstance();
};
