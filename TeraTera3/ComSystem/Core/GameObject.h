/**
 * @file GameObject.h
 * @author jupiter
 * @brief GameObjectクラスを記述したヘッダ
 */

#define NOMINMAX
#include <unordered_map>
#include <map>

#include <memory>
#include <Windows.h>
#include <typeinfo>
#include <typeindex>
#include <string_view>

#include "Helper/ConceptIsComponent.h"
#include "../../System/CVector.h"
#include "../../EventSystem/CEventSystem.h"

#pragma once

class ComponentBase;
class ComTransform;
class ObjectGenerator;
class ControlObjectPriority;

/**
 * @brief オブジェクトのタイプ
 * @details これによってデフォルトの描画順と追加される初期パッケージが決まる
 * @n 自分でコンポーネントを追加してもいい
 */
enum class E_TYPE_OBJECT
{
    /**
     * @brief 追加コンポーネントなし
     */
    NONE,

    /**
     * @brief UI関係
     * @n ComShader、Com2DTexture
     */
    UI,

    /**
      * @brief テクスチャの表示
      * @n ComShader、Com3DTexture
      */
    IMAGE,

    /**
       * @brief ビルボード（画像が常にカメラを向いているIMAGE）
       * @n ComShader、Com3DBillBoard
       */
    BILLBOARD,

    /**
        * @brief モデル、図形(Assimpは外すこと)等
        * @n ComShader、Com3DModelAssimp
        */
    MODEL3D,

    /**
         * @brief システム用
         * @n 一番初めに実行されるように
         */
    SYSTEM,
};

/**
 * @brief   ベースになるオブジェクト
 * @details 継承できるようになっているがするな！（finalにしたいが生成時にfinalだと無理なので）
 */
class GameObject
{
    /**
     * @brief コンポーネント管理用コンテナ
     * @details type_indexキーはクラスの型情報のこと
     */
    std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> m_pListComponent;

    /**
     * @brief Readyを一度通すために使用するリスト
     * @n Init時に登録
     */
    std::vector<std::type_index> m_listReadyComponent;

    /**
     * @brief 削除するコンポーネントを一時的に保持するリスト
     */
    std::vector<std::type_index> m_listUpdateComponent;

    /**
     * @brief 削除するコンポーネントを一時的に保持するリスト
     */
    std::vector<std::type_index> m_listDrawComponent;

    /**
     * @brief 削除するコンポーネントを一時的に保持するリスト
     */
    std::vector<std::type_index> m_listEraseComponent;

    /**
     * @brief 子オブジェクトを管理するためのコンテナ
     * @details Destroyを使えば削除できるが作成時のObjectをなくすと削除できないので注意
     */
    std::unordered_map<int, GameObject *> m_pListChildObject;

    /**
     * @brief 子オブジェクト呼び出し用のstring型キーを管理するためのコンテナ
     */
    std::unordered_map<std::string, int> m_pListChildObjectName;

    /**
     * @brief このオブジェクトはシーンをまたいでも存在するかどうか
     * @details 変えてもDestroyで消せます
     */
    bool m_isStraddle = false;

    /**
     * @brief 現在のシーン名
     */
    static std::string m_nowSceneName;

    /**
     * @brief m_objIDを決めるためのカウンタ
     */
    static int m_classCount;

    /**
     * @brief 親オブジェクトのアクティブかどうかを保持する変数
     */
    bool m_flagParentActive = false;

public:
    //ObjectGenetator空からのアクセスを許可
    friend ObjectGenerator;
    //ComponentBaseからのアクセスを許可
    friend ComponentBase;

    friend ControlObjectPriority;

    /**
     * @brief このオブジェクトの名前
     * @n 絶対に変えるな！
     */
    std::string m_objectName;

    /**
     * @brief オブジェクトの実行優先順位 昇順に実行される
     * @n オブジェクト自体の更新の順番に影響
     * @n 低い順に実行されます
     * @details  -30~200の範囲で値を設定してください
     */
    CVector<int> m_objectUpdatePriority;

    /**
     * @brief オブジェクトの描画優先度
     * @n 変更するのはお勧めしません
     * @details 当たり判定に使用？
     */
    CVector<int> m_objectDrawPriority;

    /**
     * @brief オブジェクトの種類
     * @n フラスタムカリングを行うかどうかのフラグにも(OBJECTと3DMODEL、3DTextureが対象)
     * @nこれによって描画順等を決定
     */
    E_TYPE_OBJECT m_typeObject = E_TYPE_OBJECT::NONE;

    /**
     * @brief 座標等の情報が入ったコンポーネントクラスへのショートカット用
     * @details ここでエラーが起こる場合そのファイルではGameObject.hしか読み込んでいないのでCode.hに置き換えてください
     */
    ComTransform *m_transform = nullptr;

    /**
     * @brief このオブジェクトはアクティブかどうか
     */
    CVector<bool> m_activeFlag;

    /**
     * @brief オブジェクトの管理用ID
     */
    const int m_objID;

    /**
     * @brief 検索等に使用するタグ
     * @todo tagの処理を追加
     */
    std::string m_tag = "NoneTag";

    /**
     * @brief 親オブジェクトアクセス用のメンバ
     * @n 変えるな！
     */
    GameObject *m_pParentObject = nullptr;

    /**
     * @brief オブジェクトを描画するレイヤー
     * @n 小さい順に描画
     */
    CVector<int> m_drawLayer;

private:
    /**
     * @brief  CObjectの生成時処理
     * @details makeNewObject以外から生成したくないのでprivateにしています
     */
    GameObject() : m_objID(m_classCount)
    {
        m_classCount++;
        Init();
    }

public:
    /**
     * @brief CObjectの破棄時処理
     */
    virtual ~GameObject()
    {
        Uninit();
    };

    /**
     * @brief   初期化処理
     * @details これは呼び出す必要なし
     */
    virtual void Init();

    /**
     * @brief オブジェクト破棄時の処理
     */
    virtual void Uninit();

    /**
     * @brief   アップデート処理
     * @details 循環参照回避のためにcppに記述
     */
    void Update();

    /**
     * @brief   描画処理
     * @details 循環参照回避のためにcppに記述
     */
    void Draw();

    /**
     * @brief このオブジェクトをシーンをまたいでも存在させる
     * @n 削除はできます
     */
    void DontDestroyOnLoad(void)
    {
        m_isStraddle = true;
    }

    //component系

    /**
     * @brief コンポーネントをセットする処理
     * @tparam component セットしたいコンポーネントクラスを入れる
     * @return component *const  セットしたコンポーネントクラスが返される
     */
    template <IsComponent component>
    component *const AddComponent()
    {
        //キーに対応する内容があるかどうかの確認
        if (!m_pListComponent.contains(typeid(component)))
        {
            //キーに対応する内容は存在していない
            std::shared_ptr<component> com = std::make_shared<component>();

            //以下はなぜアクセスできているか不思議なのでエラーが出たら別に分けて修正
            //もしかしてコンセプトで保障しているから？ or
            //Templateはコンパイル時に機能するのでその関係か

            com->m_gameObject = this;
            com->Init();
            com->SetComponentID(com.get());

            m_pListComponent[typeid(component)] = com; //ここでエラーが起こったのならcomponentを継承していないクラスが入っている

            m_listReadyComponent.push_back(typeid(component));

            //イベント確認と登録の処理 ここではcom(継承後アドレス)を渡す
            CEventSystem::GetInstance().SetComponentToEvent(this, com.get());

            return GetComponent<component>();
        }
        //既に存在しているのでnullptr
        return nullptr;
    }

    /**
     * @brief オブジェクトから指定のコンポーネントを取得する処理
     * @tparam component 取得したいコンポーネントを指定
     * @return component* const 取得したいコンポーネントのポインタ
     * @n       もしnullptrならまだセットされていないコンポーネントを指定している
     */
    template <IsComponent component>
    [[nodiscard]] component *const GetComponent()
    {
        if (m_pListComponent.contains(typeid(component)))
        {
            auto com = std::static_pointer_cast<component>(m_pListComponent[typeid(component)]);
            return com.get();
        }
        return nullptr;
    }

    /**
     * @brief このオブジェクトからコンポーネントを削除する処理
     * @tparam component 消したいコンポーネント名
     * @return true 成功した
     * @return false 失敗した
     */
    template <IsComponent component>
    bool RemoveComponent()
    {
        auto com = this->GetComponent<component>();
        return this->RemoveComponent(com);
    }

    /**
     * @brief 特定のオブジェクトからコンポーネントを削除する処理
     * @param type 削除したいコンポーネントのポインタ
     * @return true 成功
     * @return false 失敗
     */
    template <IsComponent component>
    bool RemoveComponent(component *type)
    {
        bool flag = true;

        //既にリストに登録しているか確認
        for (auto itr : m_listEraseComponent)
        {
            if (typeid(*type) == itr)
            {
                flag = false;
                break;
            }
        }

        //typeは存在しているものか
        if (type != nullptr && flag)
        {
            auto obj = type->m_gameObject;
            //コンポーネントがリストに存在するか確認
            if (obj->m_pListComponent.contains(typeid(*type)))
            {
                obj->m_listEraseComponent.push_back(typeid(*type));
                return true;
            }
        }
        return false;
    }

    /**
     * @brief このオブジェクトのコンポーネントリストを取得する（変数内の内容も）
     * @return std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> *const 取得したいコンポーネントリスト
     */
    std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> *const GetListComponent()
    {
        return &m_pListComponent;
    }

    //package系

    /**
     * @brief       コンポーネントをまとめたパッケージを追加する処理
     * @tparam component    セットしたいパッケージクラスを選択
     */
    template <class component>
    void AddPackage()
    {
        std::unique_ptr<component> package = std::make_unique<component>();
        package->Init();
        //ここでエラーが出るならそれはパッケージクラスを継承していないクラス
        std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> *packagelist = package->GetListComponent();

        for (auto &itr : *packagelist)
        {
            //既に追加されていないかどうか(すでにあるものは下のmargeではじかれる)
            if (!m_pListComponent.contains(itr.first))
            {
                itr.second->m_gameObject = this;
                itr.second->Init();
                itr.second->SetComponentID(itr.second.get());

                m_listReadyComponent.push_back(itr.first);

                //イベント確認と登録の処理 ここではcom(継承後アドレス)を渡す
                CEventSystem::GetInstance().SetComponentToEvent(this, itr.second.get());
            }
        }
        //c++17から追加機能
        //エラーが出たらプロジェクト=>c/c++->言語標準からc++17にすること
        //自信にないキーを追加する
        m_pListComponent.merge(*packagelist);
        packagelist->clear();
        package.reset();
    }

    //child系

    /**
     * @brief このオブジェクトに子オブジェクトを加える処理
     * @param type 加えたいオブジェクトのタイプ
     * @return GameObject* const 加えた子オブジェクトのポインタ
     */
    GameObject *const AddChildObject(E_TYPE_OBJECT type);

    /**
     * @brief このオブジェクトに子オブジェクトを加える処理２
     * @details このメンバではnameを設定することによりnameで探せるようになる
     * @param name 探す際に使うstring
     * @param type 加えたいオブジェクトのタイプ
     * @return GameObject* const 加えた子オブジェクトのポインタ
     */
    GameObject *const AddChildObject(std::string name, E_TYPE_OBJECT type);

    /**
     * @brief このオブジェクトから子オブジェクトをidを使い取得する処理
     * @param objid オブジェクトのid
     * @return GameObject* const 取得したい子オブジェクトが返される
     */
    [[nodiscard]] GameObject *const FindChildObject(int objid);

    /**
     * @brief このオブジェクトから子オブジェクトをその名前を使い取得する処理
     * @param name オブジェクトに設定した名前
     * @return GameObject* const 取得したい子オブジェクトが返される
     */
    [[nodiscard]] GameObject *const FindChildObject(std::string name);

    /**
     * @brief リストから指定したオブジェクトを削除する処理
     * @param obj 消したいオブジェクトのID
     * @return true 成功しました
     * @return false 失敗しました（対象存在しないかエラー発生中）
     */
    bool DestroyChild(GameObject *obj)
    {
        return DestroyChild(obj->m_objID);
    }

    /**
     * @brief リストから指定したオブジェクトを削除する処理2
     * @param objid オブジェクトIDをいれること
     * @return true 成功しました
     * @return false 失敗しました（対象存在しないかエラー発生中）
     */
    bool DestroyChild(const int objid);

    /**
     * @brief リストから指定したオブジェクトを削除する処理3
     * @details nameを設定して生成した場合のみ可能
     * @param name 消したいオブジェクトの名前
     * @return true 成功しました
     * @return false 失敗しました（対象存在しないかエラー発生中）
     */
    bool DestroyChild(std::string_view name)
    {
        return DestroyChild(m_pListChildObjectName[name.data()]);
    }

    //その他
protected:
    /**
     * @brief 削除待ちのコンポーネントを削除する
     */
    void EraseComponent(void);

    //static系
public:
    /**
     * @brief 新しいオブジェクトを作る処理
     * @param name オブジェクトの名前
     * @param type オブジェクトの初期コンポーネントのタイプ
     * @return GameObject* const オブジェクトのポインタ
     */
    static GameObject *const MakeNewObject(std::string_view name, E_TYPE_OBJECT type);

    /**
     * @brief オブジェクト検索用メソッド
     * @param objid 検索したオブジェクトのid
     * @return GameObject* const 検索したオブジェクトが返される
     */
    [[nodiscard]] static GameObject *const Find(int objid);

    /**
     * @brief オブジェクト検索用メソッド2
     * @param name 検索したいオブジェクト名を入れる
     * @return GameObject* const 検索したオブジェクトが返される
     */
    [[nodiscard]] static GameObject *const Find(std::string_view name);

    /**
     * @brief ImGuiで表示する際に使用する処理
     * @param windowid ウインドウのid
     */
    void ImGuiDraw(unsigned int windowid);

private:
    void ImGuiDrawObjectDetails(unsigned int windowid);

    void ImGuiDrawComponents(unsigned int windowid);

    void ImGuiDrawChiObjects(unsigned int windowid);

    /**
     * @brief オブジェクトを削除する処理1
     * @n おそらく1ばん早い
     * @details 子オブジェクトかどうかを確認する処理も入っている
     * @param obj 削除したいオブジェクトのポインタ
     * @return true 成功
     * @return false 失敗
     */
    static bool DestroyObject(GameObject *obj);

    /**
     * @brief オブジェクトを削除する処理2
     * @details 子オブジェクトかどうかを確認する処理も入っている
     * @param id 削除したいオブジェクトのid
     * @return true 成功
     * @return false 失敗
     */
    static bool DestroyObject(int id);

    /**
     * @brief オブジェクトを削除する処理3
     * @details 子オブジェクトかどうかを確認する処理も入っている
     * @param name 削除したいオブジェクトの名前
     * @return true 成功
     * @return false 失敗
     */
    static bool DestroyObject(std::string_view name);

    /**
     * @brief 現在のシーン名を取得する処理
     * @return std::string_view 取得したいシーン名
     */

    [[nodiscard]] static std::string_view GetNowSceneName()
    {
        return m_nowSceneName;
    }
};
