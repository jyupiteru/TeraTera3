/**
 * @file ComTitleSelectorMove.h
 * @author jupiter ()
 * @brief ComTitleSelectorMoveの宣言が書かれたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief タイトルに表示するセレクト用の画像の大きさを調整するコンポーネント
 */
class ComTitleSelectorMove : public ComponentBase
{
    /**
     * @brief 最初の大きさ
     */
    DirectX::XMFLOAT2 m_firstWidthSize = {0.0f, 0.0f};

    /**
     * @brief 現在は拡大か縮小か
     * @n 1で拡大、-1で縮小
     */
    int m_nowFlow = 1;

public:
    /**
     * @brief 現在の大きさを最小としてどれくらい大きくするか
     */
    float m_widthMaxPlusSize = 20.0f;

    /**
     * @brief 1秒あたりにどれくらい大きくするか
     */
    float m_plusSizePerSecond = 10.0f;

    ComTitleSelectorMove(){};
    ~ComTitleSelectorMove(){};

    void Ready();

    void Update();
};
