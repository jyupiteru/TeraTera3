/**
 * @file ComTitleManager.h
 * @author jupiter ()
 * @brief ComTitlaManagerの宣言が記載されたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComTitleManager : public ComponentBase
{
    /**
     * @brief 今までプレイした中での最高得点を表示するテキストへのアクセス簡略用変数
     */
    Com2DText *m_comhighScoreText = nullptr;

public:
    ComTitleManager(){};
    ~ComTitleManager(){};
    void Init() override;
    void Ready() override;
    void Update() override;
};
