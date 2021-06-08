/**
 * @file Com3DBillBoard.h
 * @author jupiter
 * @brief Com3DBillBoardが載っているヘッダ
 */
#include <memory>

#include "../../../Core/Cores.h"
#include "../Com3DTexture/Com3DTexture.h"
#include "../../System/ComCamera/ComCamera.h"

#pragma once

/**
 * @brief ビルボード表示の際に使用するクラス
 */
class Com3DBillBoard : public Com3DTexture
{
    ComCamera *m_pComCamera = nullptr;

public:
    Com3DBillBoard(){};

    ~Com3DBillBoard(){};

    virtual void Init() override;

    virtual void Uninit() override;

    virtual void Ready() override;

    virtual void Draw() override;

    virtual void ImGuiDraw(unsigned int windowid) override;

    /**
     * @brief 画像を読み込む処理
     * @n ビルボードではこちらを使用
     * @param texturename 読み込むタイテクスチャ名
     */
    void LoadTexture(std::string texturename)
    {
        Com3DTexture::LoadTexture(texturename, E_TYPE_TEXTUREOBJ::VERTICAL);
    }
};
