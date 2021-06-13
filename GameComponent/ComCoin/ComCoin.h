
#include "../../TeraTera3/TeraTera.h"

#pragma once

/**
 * @brief コインに保持させるコインの動作を行わせるクラス
 */
class ComCoin : public ComponentBase
{

public:
    /**
     * @brief コインに当たった時のスコア
     * @n 多ければ多いほど早く回る
     */
    CVector<int> m_hitScore;

public:
    ComCoin(){};
    ~ComCoin(){};

    void Update() override;

    void OnTriggerStay3D(GameObject *obj);
};
