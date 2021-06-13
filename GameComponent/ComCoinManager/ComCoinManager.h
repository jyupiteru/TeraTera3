
#include "../../TeraTera3/TeraTera.h"

#pragma once

class ComCoin;

class ComCoinManager : public ComponentBase
{
    /**
     * @brief 弾を管理するリスト
     */
    std::vector<ComCoin *> m_listCoinList;

    /**
     * @brief 生成待ちの弾
     */
    std::vector<ComCoin *> m_listWaitCreate;

    /**
     * @brief 現在アクティブな弾の配列
     */
    std::vector<ComCoin *> m_listActiveCoin;

    /**
     * @brief このクラスの実体
     */
    static ComCoinManager *m_instance;

    /**
     * @brief コインの生成周期管理用
     */
    float m_timeCount = 0.0f;

    /**
     * @brief 取得したコインの総数を数える変数
     */
    int m_coinGetCount = 0;

    /**
     * @brief 取得したコイン数を表示するテキスト
     */
    Com2DText *m_comScoreText = nullptr;

public:
    /**
     * @brief コインの大きさを管理する変数
     */
    CVector3<float> m_coinSize;

    /**
     * @brief コインの各種スコアを保存する変数
     * @n 初期値、最高
     */
    CVector2<int> m_coinScore;

    /**
     * @brief コインの生成間隔
     * @n 初期の間隔、最終の間隔
     */
    CVector2<float> m_intervalTime;

    /**
     * @brief コインの生成上限
     */
    CVector<int> m_coinMax;

public:
    ComCoinManager(){};
    ~ComCoinManager(){};

    void Init() override;
    void Uninit() override;
    void Ready() override;
    void Update() override;

    /**
     * @brief 生成待ちリストに登録する処理
     * @param _coin 登録したいコインが持つComCoinクラスのポインタ
     * @param _Flag このコインの登録する理由 trueならプレイヤーに当たったとして点数加算
     */
    void SetWaitList(ComCoin *_coin, bool _flag);

    /**
     * @brief コインをステージ上に生成する処理
     */
    void CreateCoin();

    /**
     * @brief このクラスの実体を取得する処理
     * @return ComCoinManager* クラスの実体
     */
    static ComCoinManager &GetInstance();

protected:
    /**
     * @brief コインオブジェクトを追加で生成する処理
     */
    void CreateCoinObject();
};
