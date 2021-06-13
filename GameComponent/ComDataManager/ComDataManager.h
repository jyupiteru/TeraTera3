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

    /**
     * @brief 取得した弾の総スコア
     */
    CVector<int> m_shotScore;

    /**
     * @brief プレイした中での最高得点
     */
    CVector<int> m_highScore;

public:
    ComDataManager(){};
    ~ComDataManager(){};
    void Init() override;
    void Uninit() override;

    [[nodiscard]] static ComDataManager &GetInstance();

    /**
     * @brief ハイスコアを外部から読み込む処理
     * @n Titleのみ
     */
    void LoadScore();

    /**
     * @brief ハイスコアを外部に保存する処理
     * @n Resultのみ
     */
    void SaveScore();
};
