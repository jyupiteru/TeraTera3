/**
 * @file CAssimpAnimationData.h
 * @author jupiter ()
 * @brief CAssimpAnimationDataの宣言が記載されたヘッダ
 */

#pragma once
#include "../CAssimpModelData/CAssimpScene/CAssimpScene.h"

/**
 * @brief Assimpのアニメーションモデルが格納されたクラス?
 */
class CAssimpAnimationData
{
private:
    CAssimpScene m_assimpscene;

public:
    /**
     * @brief 初期化とモデル読み込みを行うクラス
     * @param filename モデル名
     * @return true 成功
     * @return false 失敗
     */
    bool Init(std::string filename);

    /**
     * @brief 読み込んだ情報を取得する処理
     * @return const aiScene* 取得したいモデル変換済み情報
     */
    const aiScene *GetScene()
    {
        return m_assimpscene.GetScene();
    }

    /**
     * @brief 破棄処理
     */
    void Uninit();
};