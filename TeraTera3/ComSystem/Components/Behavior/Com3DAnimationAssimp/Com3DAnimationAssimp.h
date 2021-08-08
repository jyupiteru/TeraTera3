/**
 * @file Com3DAnimationAssimp.h
 * @author jupiter ()
 * @brief Com3DAnimationAssimpクラスの宣言が書かれたヘッダ
 */

#include "../../Behavior/Com3DModelAssimp/Com3DModelAssimp.h"
#include "../../../Core/ComponentBase.h"
#include "../../../../System/AssimpModel/CAssimpAnimationData/CAssimpAnimationData.h"
#include "../../../../Managers/ResourceContainer/CContainer.h"

#pragma once

class Com3DModelAssimp;

/**
 * @brief モデルのアニメーション情報等
 * @details 各種ファイルごとに仕分け
 */
struct taglistAssimpAnimation
{
    /**
         * @brief モデルのアニメーション情報(グループ単位)
         * @n アニメーショングループの名前、そのグループのアニメーションの情報
         */
    std::unordered_map<std::string, std::shared_ptr<CAssimpAnimationData>> m_listAnimation;

    /**
         * @brief 各アニメーション名を格納したリスト
         * @n アニメーション名、アニメーショングループの名前、何個目のアニメーションか
         */
    std::unordered_map<std::string, std::pair<std::string, int>> m_listAnimationName;

    taglistAssimpAnimation(){};
    ~taglistAssimpAnimation()
    {
        m_listAnimationName.clear();

        for (auto itr = m_listAnimation.begin(); itr != m_listAnimation.end();)
        {
            itr->second.reset();
            itr = m_listAnimation.erase(itr);
        }
        m_listAnimation.clear();
    }
};

/**
 * @brief Com3DModelAssimpで読み込んだモデルをアニメーションさせる際に必要になるクラス
 * @n Com3DModelAssimpを先に読み込むこと！
 */
class Com3DAnimationAssimp : public ComponentBase
{

    /**
     * @brief アニメーションデータをモデルごとに格納、管理する変数
     */
    static CListResource *m_listAssimpAnimation;

    /**
     * @brief クラス生成数のカウンタ
     */
    static unsigned int m_classCount;

    /**
     * @brief 現在再生しているアニメーションの情報
     * @n グループ名、グループ内での番号
     */
    std::pair<std::string, int> m_nowAnimation = {"None", -1};

    /**
     * @brief モデルの情報を持つコンポーネントへのパス
     */
    Com3DModelAssimp *m_pCom3DModel = nullptr;

    /**
     * @brief 現在のモデルのアニメーション情報へのポインタ
     */
    taglistAssimpAnimation *m_pNowAnimationData = nullptr;

public:
    Com3DAnimationAssimp(){};
    ~Com3DAnimationAssimp(){};

    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Ready() override;
    virtual void Update() override;
    virtual void ImGuiDraw(unsigned int windowid) override;

    /**
     * @brief モデルのアニメーションを読み込む処理
     * @n アニメーションは読み込んだモデルの初めのものに設定されるので適時変更すること
     * @param groupname このアニメーションのグループ(アニメーションが複数入っている可能性あり)にセットする名前
     * @param animationpass アニメーションまでのパス
     * @return 0以上なら成功、そのアニメーショングループのアニメーション総数を返す
     * @n -1なら失敗 or 読み込んだことがある
     */
    int LoadAnimation(std::string_view groupname, std::string_view animationpass);

    /**
     * @brief 各アニメーションに名前を割り振る処理
     * @param groupname セットしたいアニメーションが属しているグループの名前(Loadでセットしたやつ)
     * @param num いくつ目のアニメーションに名前を設定するか
     * @param animationname アニメーションの名前
     * @return true 成功した(アニメーションは存在する)
     * @return false 失敗した(アニメーションは存在しない)
     */
    bool SetAnimationName(std::string_view groupname, unsigned int num, std::string_view animationname);

    /**
     * @brief このモデルのアニメーショングループの総数を取得するメソッド
     * @return int いくつ存在するか
     * @n -1でまだ読み込めていない
     */
    [[nodiscard]] int GetAnimationGroupTotal();

    /**
     * @brief このモデルの動いているアニメーションが属しているグループ内の総数
     * @return int いくつアニメーションが存在しているか
     * @n -1でまだモデルを読み込めていない
     */
    [[nodiscard]] int GetAnimationInGroupTotal();

    /**
     * @brief このモデルの引数のグループ内にいくつアニメーションが存在するかを取得するメソッド
     * @param groupname 取得したいアニメーショングループ名
     * @return int いくつ存在しているか
     * @n -1でまだモデルの読み込みなし
     */
    [[nodiscard]] int GetAnimationInGroupTotal(std::string groupname);

    /**
     * @brief 現在動かしているアニメーションは終わったか
     * @details 正確には現在最後のアニメーションを実行しているか
     * @return true 終了した
     * @return false まだ終わっていない
     */
    [[nodiscard]] bool IsFinish();

    /**
     * @brief 同じグループ内のアニメーションに変更するメソッド
     * @param animationname 変更したいアニメーション名
     * @return true 変更に成功した
     * @return false 変更に失敗
     */
    bool ChangeAnimation(std::string animationname);

    /**
     * @brief アニメーションを簡易的に変更するための処理
     * @param groupname 変更したいアニメーションの属するグループ
     * @param num 何個目のアニメーション化
     * @return true 変更に成功
     * @return false 変更に失敗
     */
    bool ChangeAnimation(std::string groupname, int num);

    /**
     * @brief フレームを0に戻す処理
     */
    void ResetFrame();
};
