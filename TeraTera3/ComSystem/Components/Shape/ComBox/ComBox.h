/**
 * @file ComBox.h
 * @author jupiter ()
 * @brief ComBoxを記載したヘッダ
 */

#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <math.h>
#include "../../../Core/ComponentBase.h"
#include "../../Behavior/ComShader/ComShader.h"

/**
 * @brief 四角形の３D モデルを生成、保持するコンポーネント
 */
class ComBox : public ComponentBase
{
protected:
	/**
	 * @brief 頂点バッファ
	 */
	static ID3D11Buffer *m_pVertexBuffer;

	/**
	 * @brief インデックスバッファ
	 */
	static ID3D11Buffer *m_pIndexBuffer;

	/**
	 * @brief クラスのカウンタ
	 */
	static unsigned int m_comCount;
	/**
	 * @brief 横幅(X)
	 */
	float m_lengthx = 1.0f;

	/**
	 * @brief 高さ(Y)
	 */
	float m_lengthy = 1.0f;

	/**
	 * @brief 奥行き(Z)
	 */
	float m_lengthz = 1.0f;

	/**
	 * @brief 保持しているComShaderへの簡易アクセス用
	 */
	ComShader *m_pComShader = nullptr;

	/**
	 * @brief 頂点の情報を保存する構造体
	 */
	struct tagVertex
	{
		/**
		 * @brief 各座標
		 */
		DirectX::XMFLOAT3 Pos = {0.0f, 0.0f, 0.0f};

		/**
		 * @brief 正規化したベクトル？
		 */
		DirectX::XMFLOAT3 Normal = {0.0f, 0.0f, 0.0f};

		/**
		 * @brief 色情報
		 */
		DirectX::XMFLOAT4 Color = {0.0f, 0.0f, 0.0f, 0.0f};
	};

	/**
	 * @brief インデックスのデータを格納する構造体
	 */
	struct tagFace
	{
		unsigned int idx[3] = {0, 0, 0};
	};

	/**
	 * @brief 頂点の座標等を入れる変数
	 */
	static tagVertex m_vertex[8];

	/**
	 * @brief  面のインデックスデータ
	 */
	tagFace m_face[12];

	/**
	 * @brief 面の数
	 */
	unsigned int m_facenum = 12;

	/**
     * @brief 影の描画対象にするかを指定するフラグ 描画するならReady前にtrueにすること
     */
	bool m_flagDrawShadow = false;

public:
	ComBox(){};

	virtual ~ComBox(){};

	virtual void Init() override;

	virtual void Ready() override;

	virtual void Uninit() override;

	virtual void Draw() override;

	virtual void ImGuiDraw(unsigned int windowid) override;

protected:
	/**
	 * @brief インデックスデータ（おそらく順番）を作成するメソッド
	 */
	void CreateIndex();

	/**
	 * @brief 頂点座標を作成するメソッド
	 */
	void CreateVertex();

	/**
	 * @brief 色を変更する（反映させる）メソッド
	 */
	virtual void ChangeColor();

	/**
	 * @brief 正規化処理を行うメソッド
	 * @param vector 正規化したいベクトル型変数
	 * @param Normal 正規化後の変数
	 */
	void Normalize(DirectX::XMFLOAT3 vector, DirectX::XMFLOAT3 &Normal);

	/**
     * @brief 影を描画するのに使用する処理
     */
	void DrawShadow() override;
};