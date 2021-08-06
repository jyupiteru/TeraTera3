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
 * @brief マップの情報を管理するコンポーネント
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

    virtual void Ready();

    virtual void Update();

    virtual void Draw();
};