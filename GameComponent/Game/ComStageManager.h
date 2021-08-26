/**
 * @file ComStageManager.h
 * @author jupiter ()
 * @brief ComStageManagerクラスの宣言のあるヘッダ
 */

#include "../../TeraTera3/TeraTera.h"
#include <unordered_map>
#pragma once

/**
 * @brief 現在使用しているステージの情報を管理するコンポーネント
 */
class ComStageManager : public ComponentBase
{
public:
    /**
     * @brief ステージを管理する変数
     * @n 縦、横、奥、オブジェクトの管理
     */
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, GameObject>>> m_listStageDate;

    virtual void Init();
    virtual void Uninit();

    /**
     * @brief ステージを生成するメソッド
     * @param num 生成したいマップのステージ番号
     * @n 1～
     */
    void MakeStage(int num);
};