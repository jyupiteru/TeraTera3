/**
 * @file ControlObjPriority.h
 * @author jupiter ()
 * @brief ControlObjPriorityの書かれているヘッダ
 */

#include <map>
#include <algorithm>

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
    ControlObjectPriority() {};
    virtual ~ControlObjectPriority()
    {
        m_listObjectForUpdate.clear();
        ResetDrawList();
    }

protected:
    /**
     * @brief 描画順の基準を決める際に使用するカメラオブジェクト
     */
    static GameObject* m_pCameraObject;

    /**
     * @brief オブジェクトの実行優先度を管理するコンポーネント
     * @n   実行優先度、ObjectIDの順番
     * @details 自動で1個めのソートをしてくれる
     */
    static std::multimap<int, int> m_listObjectForUpdate;

    /**
     * @brief カメラからの距離と半透明、不透明なオブジェクトを入れて管理する変数
     * @n レイヤー、半透明か不透明化、距離、描画優先度、ID
     * @details 近くから順に描画(Zソート法)
     * @n なので半透明は逆イテレータを使用すること(Zバッファ法)
     * @n レイヤーは0が一番下
     */
    static std::map<int, std::map<int, std::map<float, std::multimap<int, int>>>> m_listObjectForDraw;

    //以下継承したObjectGeneratorで使用するため本当はいらないがわかりやすくするために作成

    static void SetObjectUpdateOrder(const GameObject& obj);

    /**
     * @brief 描画順を登録する処理
     * @param obj 登録したいオブジェクトのアドレス
     * @param distance カメラからの距離
     */
    static  void SetObjectDrawingOrder(const GameObject& obj, const float distance);

    /**
     * @brief アップデート時に使用するリストをリセットするメソッド
     */
    static  void ResetUpdateList();

    /**
     * @brief 描画時に使用するリストをリセットする処理
     */
    static void ResetDrawList();
};
