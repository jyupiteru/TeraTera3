/**
 * @file ComMapMaker.h
 * @author jupiter ()
 * @brief ComMapMakerの宣言が書かれたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#include <unordered_map>

#pragma once

/**
 * @brief ステージ作成時に使用する列挙型
 */
enum class E_MAPCHIP
{
    NONE = 0,
    START,
    GOAL,
    FLOOR,
    FLOOR_FALL,

};

/**
 * @brief マップの設計図情報の保持とマップの生成をするコンポーネント
 */
class ComMapMaker : public ComponentBase
{
    /**
     * @brief すべてのマップの設計図を管理する変数
     * @n ステージ数、縦、奥、横、種類
     * @n 下、左、手前から順に描画
     */
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, E_MAPCHIP>>>> m_mapsData;

public:
    /**
     * @brief マップに使用するオブジェクトの1辺当たりの大きさ
     */
    CVector<float> m_mapSize;

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Ready() override;

    virtual void Update() override;

    virtual void Draw() override;

    /**
     * @brief マップを生成するメソッド
     * @param num 生成したいマップのステージ番号
     * @n 1～
     */
    void MakeMap(int num);

private:
    /**
     * @brief 各マップの情報をセットする関数
     */
    void SetMap();
};