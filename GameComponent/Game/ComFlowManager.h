/**
 * @file ComFlowManager.h
 * @author jupiter ()
 * @brief ComFlowManagerの宣言を記述したヘッダ
 */
#include "../../TeraTera3/TeraTera.h"

#pragma once

enum class E_FLOW
{
    READY = 0,
    GAME,
    GOAL,
    FAILED
};

class ComTimer;

/**
 * @brief ゲーム自他の流れを管理するコンポーネント
 */
class ComFlowManager : public ComponentBase
{
    /**
     * @brief ゲームの残りの制限時間を取得するためのコンポーネントへの子インタ
     */
    ComTimer *m_pComTimer = nullptr;

    /**
     * @brief プレイヤーが落下しているか確認するために追加
     */
    GameObject *m_pPlayer = nullptr;

    /**
     * @brief 現在のゲームの状況
     */
    E_FLOW m_nowFlow = E_FLOW::READY;

    /**
     * @brief コンポーネントの実体
     */
    static ComFlowManager *m_instance;

public:
    void Init() override;
    void Uninit() override;
    void Ready() override;
    void Update() override;

    /**
     * @brief ゲームのフローを移行する
     * @param flow どのフローに移行したいか
     */
    void ChangeFlow(E_FLOW flow);

    /**
     * @brief 実体を取得する処理
     * @return ComFlowManager* このコンポーネントの実体
     */
    static ComFlowManager &GetInstance();
};