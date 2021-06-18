/**
 * @file CAssimpScene.h
 * @author jupiter ()
 * @brief CAssimpSceneクラスの宣言を記載したヘッダ
 */

#pragma once
#include <string>
#include <assimp/cimport.h>

#pragma once

/**
 * @brief Assimpで読み込んだ元モデルデータを保持するクラス?
 */
class CAssimpScene
{
private:
    /**
     * @brief Assimpで読み込んだデータ 変換済み
     */
    const aiScene *m_Scene = nullptr; // assimp scene

public:
    /**
     * @brief 初期処理 モデルを読み込む
     * @param _filename 読み込みたいモデルの名前
     * @return true 成功
     * @return false 失敗
     */
    bool Init(std::string_view _filename);

    /**
     * @brief 終了処理
     */
    void Exit();

    /**
     * @brief 読み込んだ変換済みデータを取得する処理
     * @return const aiScene* 取得したい変換済みデータのポインタ
     */
    const aiScene *GetScene() const;

    /**
     * @brief アニメーションを持っているかを判断する関数
     * @return true 持っている
     * @return false 持っていない
     */
    bool HasAnimation();
};
