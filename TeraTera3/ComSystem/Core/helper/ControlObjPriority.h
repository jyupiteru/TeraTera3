/**
 * @file ControlObjPriority.h
 * @author jupiter ()
 * @brief ControlObjPriorityの書かれているヘッダ
 */

#include <map>
#include <algorithm>
#include <vector>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#pragma once

class GameObject;

/**
 * @brief ObjectGeneratorに継承させるオブジェクト実行順管理用クラス
 * @details 複数作れないように各変数をstatic化してます
 */
class ControlObjectPriority
{
public:
    ControlObjectPriority(){};
    virtual ~ControlObjectPriority()
    {
        m_listObjectForUpdate.clear();
        ResetDrawList();
        m_listObjectNonActive.clear();
        m_listObjectWaitAddUpdate.clear();
    }

protected:
    /**
     * @brief 描画順の基準を決める際に使用するカメラオブジェクト
     */
    GameObject *m_pCameraObject = nullptr;

    /**
     * @brief オブジェクトの実行優先度を管理するコンポーネント
     * @n   実行優先度、ObjectIDの順番
     * @details 自動で1個めのソートをしてくれる
     */
    std::multimap<int, int> m_listObjectForUpdate;

    /**
     * @brief カメラからの距離と半透明、不透明なオブジェクトを入れて管理する変数
     * @n レイヤー、半透明か不透明化、距離、描画優先度、ID
     * @details 近くから順に描画(Zソート法)
     * @n なので半透明は逆イテレータを使用すること(Zバッファ法)
     * @n レイヤーは0が一番下
     */
    std::map<int, std::map<int, std::map<float, std::multimap<int, int>>>> m_listObjectForDraw;

    /**
     * @brief 生成されUpdateリストに追加を待っているオブジェクトのリスト
     */
    std::vector<int> m_listObjectWaitAddUpdate;

    /**
     * @brief 非アクティブなオブジェクトを格納するリスト
     * @n 格納されたオブジェクトはUpdateの前に確認されアクティブになっていたらUpdateリストに追加される
     */
    std::vector<int> m_listObjectNonActive;

    //以下継承したObjectGeneratorで使用するため本当はいらないがわかりやすくするために作成

    void SetObjectUpdateOrder(const GameObject &obj);

    /**
     * @brief 描画順を登録する処理
     * @param obj 登録したいオブジェクトのアドレス
     * @param distance カメラからの距離
     */
    void SetObjectDrawingOrder(const GameObject &obj, const float distance);

    /**
     * @brief Updateのリストからアクティブでなくなったオブジェクトを非アクティブリストに登録する処理
     * @n Updateリストに登録する処理で分岐してこっちに登録
     * @param _objid 登録したいオブジェクトのID
     */
    void SetObjectNonActive(int _objid);

    /**
     * @brief 生成後Updateリストに追加待ちのオブジェクトを登録する処理
     * @param _objid 追加したいオブジェクトのID
     */
    void SetObjectToWaitList(int _objid);

    /**
     * @brief 非アクティブなリストからアクティブに変わったオブジェクトをUpdateリストに移す処理
     * @n Updateのぶん回し前に使用する
     */
    void UpdateListNonActive();

    /**
     * @brief Updateリストの更新処理
     * @n すべてのObjectのUpdate前に呼ぶこと
     */
    void UpdateListObjectUpdate();

    /**
     * @brief アップデート時に使用するリストをリセットするメソッド
     */
    void ResetUpdateList();

    /**
     * @brief 描画時に使用するリストをリセットする処理
     */
    void ResetDrawList();

    /**
     * @brief 非アクティブリストからオブジェクトを削除する処理
     * @param _objid 削除したいオブジェクトのID
     */
    void EraseObjectFromListNonActive(int _objid);
};
