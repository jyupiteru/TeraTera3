
#include "../../TeraTera3/TeraTera.h"
#include "../ComShot/ComShot.h"

#pragma once

class ComShotManager : public ComponentBase
{
    /**
     * @brief 弾を管理するリスト
     */
    std::vector<ComShot *> m_listShotList;

    /**
     * @brief 生成待ちの弾
     */
    std::vector<ComShot *> m_listWaitCreate;

    static ComShotManager *m_instance;


    /**
     * @brief 対象への座標簡易アクセス用
     */
    GameObject *m_playerobj = nullptr;

    /**
     * @brief 弾の生成周期管理用
     */
    float m_timeCount = 0.0f;

public:
    /**
     * @brief 弾の生成間隔
     */
    CVector<float> m_intervalTime;

    /**
     * @brief 同時生成数
     */
    CVector<int> m_shotMax;

    /**
     * @brief 弾のスピード
     * @n 最低値、最大値
     */
    CVector2<float> m_shotSpeed;

    /**
     * @brief 弾をどれくらいブレさせるか 大きさにより難易度変わりそう
     * @n 初期 0～10
     */
    CVector<float> m_shotRandNum;

    ComShotManager(){};
    ~ComShotManager(){};

    void Init() override;

    void Uninit() override;

    void Ready() override;

    void Update() override;

    [[nodiscard]] static ComShotManager &GetInstance();

    void SetShotReadyList(ComShot *_shot);

    void CrateShot();
};
