/**
 * @file Com3DModel.h
 * @author jupiter
 * @brief Com3DModelクラスが記述されたヘッダ
 * @version 1.0
 * @date 2020-08-08
 */

#include <d3d11.h>
#include <unordered_map>
#include < memory>

#include "../../../Core/Cores.h"
#include "../ComShader/ComShader.h"

#include "../../../../WindowsSystem/Model/DatFIle/DatFile.h"
#include "../../../../ResourceContainer/CContainer.h"

#pragma once

/**
 * @brief モデルの処理に関するクラス
 */
class Com3DModel : public ComponentBase
{
private:
    /**
     * @brief モデルを管理するコンテナ
     */
    static std::unordered_map<std::string, std::shared_ptr<DatFile>> m_pListModel;

    /**
     * @brief 生成したクラスのカウンタ
     */
    static int m_classCount;

    /**
     * @brief このクラスのモデルへのキー
     */
    std::string m_keyModel;

    ComShader *m_pShader = nullptr;

public:
    Com3DModel()
    {
        m_classCount++;
    }

    virtual ~Com3DModel(){};

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Ready() override;

    virtual void Draw() override;

    virtual void ImGui_Draw(unsigned int windowid) override;

    /**
     * @brief モデル読み込み処理
     * @param modelname 読み込みたいモデルの名前をassetsフォルダより指定
     */
    void LoadModelData(std::string modelname);

    /**
     * @brief モデルのキー取得処理
     * @return std::string このクラスのアクセスキー
     */
    [[nodiscard]] std::string GetModelKey()
    {
        return m_keyModel;
    }

    /**
     * @brief モデル情報を直に取得する関数
     * @return DatFileLoader* このコンポーネントが保持しているモデル情報へのポインタ
     */
    [[nodiscard]] DatFileLoader *GetModelData()
    {
        return m_pListModel[m_keyModel]->GetDatFileLoader();
    }
};
