/**
 * @file ComMapManager.h
 * @author jupiter ()
 * @brief ComMapManagerクラスの宣言のあるヘッダ
 */

#include "../../TeraTera3/TeraTera.h"
#include <unordered_map>
#pragma once

class ComMapMaker;

/**
 * @brief 現在使用しているマップの情報を管理するコンポーネント
 */
class ComMapManager : public ComponentBase
{
public:
    friend ComMapMaker;

    /**
     * @brief マップを管理する変数
     * @n 縦、横、奥、オブジェクトの管理
     */
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, GameObject>>> m_listMapDate;

    virtual void Init();
    virtual void Uninit();

    /**
     * @brief マップを生成するメソッド
     * @param num 生成したいマップのステージ番号
     * @n 1～
     */
    void MakeMap(int num);
};