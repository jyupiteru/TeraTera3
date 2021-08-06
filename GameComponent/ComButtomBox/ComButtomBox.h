/**
 * @file ComButtomBox.h
 * @author jupiter ()
 * @brief ComButtomBoxの宣言が書かれたヘッダ
 */

#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief ゲームの落下時の終了条件に使用するコンポーネント
 */
class ComButtomBox final : public ComponentBase
{

public:
    ComButtomBox(){};
    ~ComButtomBox(){};

    void OnTriggerStay3D(GameObject *obj);
};
