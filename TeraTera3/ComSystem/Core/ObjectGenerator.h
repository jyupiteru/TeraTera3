/**
 * @file ObjectGenerator.h
 * @author jupiter
 * @brief ObjectGeneratorが載っているヘッダ
 */

#include "Helper/ControlObjPriority.h"
#include "Helper/CTagManager.h"
#include <unordered_map>
#include <memory>
#include "GameObject.h"

//前方宣言
class GameObject;
class ComCamera;

/**
 * @brief コンポーネントの初期Update順位
 */
enum class E_DEFAULT_UPDATEPRIORITY
{
    SYSTEM = -20,
    UI = 0,
    MODEL = 0,
    IMAGE = 0,
    BILLBOARD = 0,
    NONE = 0
};

#pragma once

/**
 * @brief Objectの生成器
 * @details staticで実質シングルトン化している
 * @todo 各処理の分散（クラス化してメンバ変数化）を考える
 */
class ObjectGenerator final : public ControlObjectPriority
{
    /**
     * @brief オブジェクトのリスト
     * @n id、オブジェクト
     * @details すべてのオブジェクトが入っている
     */
    static std::unordered_map<int, std::unique_ptr<GameObject>> m_pListAllObject;

    /**
     * @brief オブジェクト呼び出し用の名前を管理するためのコンテナ
     * @n オブジェクト名、id
     */
    static std::unordered_map<std::string, int> m_pListObjectName;

    /**
     * @brief 削除するオブジェクトのIDを格納する
     * @details Drawのあと削除する
     */
    static std::vector<int> m_listEraseObject;

    /**
     * @brief タグを管理するリスト、操作を持つ変数
     */
    static std::unique_ptr<CTagManager> m_cTagManager;

    /**
     * @brief フラスタムカリングを行う際に使用するカメラを持つクラス
     */
    static ComCamera *m_pComCamera;

    /**
     * @brief そのフレーム内で更新したオブジェクト計測用の変数
     */
    static unsigned int m_updateCounter;

    /**
     * @brief そのフレーム内で描画したオブジェクトの計測用の変数
     */
    static unsigned int m_drawCounter;

    /**
     * @brief 更新にかかった時間計測用の変数
     */
    static float m_updateTime;

    /**
     * @brief 描画にかかった時間計測用の変数
     */
    static float m_drawTime;

public:
    //GameObjectからのprivateのアクセスを許可
    friend GameObject;

    ObjectGenerator()
    {
        Init();
    };

    virtual ~ObjectGenerator()
    {
        Uninit();
    };

    /**
     * @brief   初期化処理
     * @details これは呼び出す必要なし
     */
    void Init();

    /**
     * @brief オブジェクト破棄時の処理
     */
    void Uninit();

    /**
     * @brief   アップデート処理
     */
    static void Update();

    /**
     * @brief   描画処理
     */
    static void Draw();

    /**
     * @brief 削除待ちのオブジェクトを削除する
     */
    static void EraseObject();

    /**
     * @brief シーンが入れ替わるときの処理
     * @param scenename 次のシーンを入れる
     * @details 消さないようにしているオブジェクト以外のすべてのオブジェクトが消されます
     */
    static void ChangeScene(std::string_view scenename);

private: //GameObjectからのみアクセス可能
    /**
     * @brief オブジェクトを生成する処理１
     * @param type E_TYPE_OBJECT型 オブジェクトの種類を指定する
     * @return GameObject *const 加えた子オブジェクトのポインタ
     * @details deleteできないが入れ替えはできるのでするな！
     */
    static GameObject *const AddObjectInGenerator(E_TYPE_OBJECT type);

    /**
     * @brief オブジェクト生成処理２ 名前が必要
     * @param name オブジェクト名
     * @param type E_TYPE_OBJECT型 オブジェクトの種類を指定する
     * @return  GameObject *const 加えた子オブジェクトのポインタ
     */
    static GameObject *const AddObjectInGenerator(std::string_view name, E_TYPE_OBJECT type);

    /**
     * @brief オブジェクトをIDから取得する処理
     * @param objid 検索したいオブジェクトのid
     * @return GameObject* const 指定したオブジェクトが返される
     */
    [[nodiscard]] static GameObject *const FindInGenerator(int objid);

    /**
     * @brief オブジェクトを名前から取得する処理
     * @param name オブジェクトに設定した名前
     * @return GameObject* const 指定したオブジェクトが返される
     */
    [[nodiscard]] static GameObject *const FindInGenerator(std::string_view name);

    /**
     * @brief リストから指定したオブジェクトを削除する処理1
     * @n 一番軽い削除処理
     * @param obj オブジェクトポインタをいれること
     * @return true 削除に成功
     * @return false 削除に失敗
     */
    static bool DestroyInGenerator(GameObject *obj);

    /**
     * @brief リストから指定したオブジェクトを削除する処理2
     * @param objid オブジェクトIDをいれること
     * @return true 削除に成功
     * @return false 削除に失敗
     */
    static bool DestroyInGenerator(const int objid)
    {
        return DestroyInGenerator(m_pListAllObject[objid].get());
    }

    /**
     * @brief リストから指定したオブジェクトを削除する処理3
     * @param name 消したいオブジェクトの名前
     * @return true 削除に成功
     * @return false 削除に失敗
     */
    static bool DestroyInGenerator(std::string name);

public:
    /**
     * @brief ImGuiでの描画用メソッド
     */
    void ImGuiDraw(int windowid);

    /**
     * @brief ImGuiで各オブジェクトを描画する用のメソッド
     */
    void ImGuiDraw_Objects(int windowid);
};
