/**
 * @file Com3DModelAssimp.h
 * @author jupiter
 * @brief Com3DModelAssimpクラスが記述されたヘッダ
 */

#include <d3d11.h>
#include <unordered_map>
#include < memory>

#include "../../../Core/ComponentBase.h"
#include "../ComShader/ComShader.h"

// #include "../../../../WindowsSystem/Model/Assimp/ModelData/ModelData.h"
#include "../../../../System/AssimpModel/CAssimpModelData/CAssimpModelData.h"
#include "../../../../ResourceContainer/CContainer.h"

#pragma once

class Com3DAnimationAssimp;

/**
 * @brief Assimpの各種情報を管理する時用の構造体
 */
struct tagAssimpModelData
{
    /**
         * @brief モデルの情報
         */
    CAssimoModelData modeldata;

    /**
         * @brief 各軸の最大値
         */
    float max_x = 0.0f, max_y = 0.0f, max_z = 0.0f;

    /**
         * @brief 各軸の最小値
         */
    float min_x = 0.0f, min_y = 0.0f, min_z = 0.0f;

    /**
         * @brief 各軸の中心の値
         */
    float center_x = 0.0f, center_y = 0.0f, center_z = 0.0f;

    /**
         * @brief x,y,z軸の中で最も大きい半径？
         */
    float sizeforrate = 0.0f;
};

/**
 * @brief Assimpモデルの処理に関するコンポーネント
 */
class Com3DModelAssimp : public ComponentBase
{
private:
    /**
     * @brief モデル等のデータを管理するコンテナ
     */
    static CListResource *m_pListModel;

    /**
     * @brief 生成したクラスのカウンタ
     */
    static int m_classCount;

    /**
     * @brief このクラスのモデルへのキー
     */
    std::string m_keyModel;

    /**
     * @brief 現在のモデル情報
     */
    tagAssimpModelData *m_pNowModelData = nullptr;

    /**
     * @brief アニメーションの際等の際に必要になる情報を集めた変数
     */
    tagAssimpAnimationData m_animationData;

    /**
     * @brief シェーダの簡易アクセス用
     */
    ComShader *m_pShader = nullptr;

    /**
     * @brief モデルの中心座標を移動させ合わせた行列
     * @n ほかのところで使うのは難しい
     */
    DirectX::XMFLOAT4X4 m_modelMatrix;

public:
    friend Com3DAnimationAssimp;

    /**
     * @brief 初期角度
     * @details 読み込んだ時にモデルの方向がおかしいときに使用
     * @todo 処理が思いつかず未実装のため注意
     */
    CVector3<float> m_firstAngle;

    /**
     * @brief モデルのローカル座標
     * @n 正確にはモデルのワールド0,0,0を基準としたローカルの座標
     * @n 値は統一後のものとする
     */
    CVector3<float> m_localPosition;

    /**
     * @brief  Com3DModelの生成時処理
     */
    Com3DModelAssimp(){};

    virtual ~Com3DModelAssimp(){};

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Ready() override;

    virtual void Update() override;

    virtual void Draw() override;

    virtual void ImGuiDraw(unsigned int windowid) override;

    /**
     * @brief モデル読み込み処理
     * @param modelname 読み込みたいモデルの名前をassetsフォルダより指定
     * @param texturefolder テクスチャを格納しているフォルダ
     */
    void LoadModelData(std::string modelname, std::string texturefolder);

    /**
     * @brief モデルのキー取得処理
     * @return std::string このモデルのアクセスキー（読み込みパス）
     */
    [[nodiscard]] std::string GetModelKey()
    {
        return m_keyModel;
    }

    /**
     * @brief モデルの情報を取得する処理
     * @return std::shared_ptr<ModelData>& 取得したいモデルの情報
     */
    [[nodiscard]] tagAssimpModelData const &GetModelData()
    {
        return *m_pNowModelData;
    }

protected:
    /**
     * @brief モデルの大きさの比率を計算する処理
     */
    void SetModelRate();

    /**
     * @brief 大きい、小さい値を比較したい値と比較し大きい、小さければ入れ替える関数
     * @param num 比較したい値
     * @param nowmax 現在の大きい値
     * @param nowmin 現在の小さい値
     */
    void CheckVolume(float num, float &nowmax, float &nowmin);

    /**
     * @brief 影を描画する際に使用する処理
     */
    void DrawShadow();
};
