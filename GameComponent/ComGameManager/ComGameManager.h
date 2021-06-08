/**
 * @file ComGameManager.h
 * @author jupiter ()
 * @brief ComGameManagerクラスの宣言が書かれたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief ゲームのフローの列挙型
 */
enum class E_GAMEFLOW
{
    READY,
    GAME,
    END
};

/**
 * @brief ゲームシーンの流れを管理するコンポーネントクラス
 */
class ComGameManager : public ComponentBase
{
    static ComGameManager *m_instance;

public:
    /**
     * @brief 現在のゲームのフロー
     */
    CVector<E_GAMEFLOW> m_nowGameState;

    ComGameManager(){};
    ~ComGameManager(){};

    void Init() override;
    void Uninit() override;
    void Update() override;

    [[nodiscard]] static ComGameManager &GetInstance();

private:
    /**
     * @brief ゲームフローの更新処理
     */
    void UpdateFlow();
};
