
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
     * @n 初期の間隔、最終の間隔
     */
    CVector2<float> m_intervalTime;

    /**
     * @brief 弾のスピード
     * @n 最低値、初期最大値、最後の最大値
     */
    CVector3<float> m_shotSpeed;

    /**
     * @brief 弾の大きさ
     * @n 1を基準に比率
     */
    CVector3<float> m_shotSize;

    /**
     * @brief 弾の大きさの変化率
     * @n 最小値、初期最大値、最終最大値
     */
    CVector3<float> m_shotSizeRate;

    /**
     * @brief 弾をどれくらいブレさせるか 大きさにより難易度変わりそう
     * @n 最小値、初期最大値、最終最大値
     */
    CVector3<float> m_shotRandNum;

public:
    ComShotManager(){};
    ~ComShotManager(){};

    void Init() override;
    void Uninit() override;
    void Ready() override;
    void Update() override;

    [[nodiscard]] static ComShotManager &GetInstance();

    /**
     * @brief 弾を生成待ちリストに追加する処理
     * @param _shot 登録したいオブジェクトに追加しているComShotコンポーネント
     */
    void SetShotReadyList(ComShot *_shot);

    /**
     * @brief 弾オブジェクトをアクティブにして速度等をセットする処理
     */
    void CrateShot();

private:
    /**
     * @brief 弾オブジェクトの生成
     */
    void CreateShotObject();
};
