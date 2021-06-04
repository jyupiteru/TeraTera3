
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
     * @brief 生成をする座標
     */
    std::vector<std::pair<float, float>> m_listFirstPos;

    /**
     * @brief 対象への座標簡易アクセス用
     */
    GameObject *m_playerobj = nullptr;

public:
    /**
     * @brief 弾の生成間隔
     */
    CVector<float> m_intervalTime;

    /**
     * @brief 同時生成数
     */
    CVector<int> m_shotMax;

    CVector<float> m_shotSpeed;

    ComShotManager(){};
    ~ComShotManager(){};

    void Init() override;

    void Uninit() override;

    void Ready() override;

    void Update() override;

    [[nodiscard]] static ComShotManager &GetInstance();

    void SetShotReadyList(ComShot *_shot);

    void SetShotFirstPos(std::vector<std::pair<float, float>> &_firstpos);
};
