/**
 * @file CMatrix.h
 * @author jupiter
 * @brief 	行列管理、計算用のクラスの書いてあるヘッダ
 * @version 1.1
 * @date 2020-07-16
 */

#include <DirectXMath.h>

#include "../../WindowsSystem/Dx11mathutil/Dx11mathutil.h"

#pragma once

/*!
@brief		行列に関する処理をするクラス
@details	座標、角度等々
*/
class CMatrix
{
	//!@brief		基準点から見た座標
	DirectX::XMFLOAT3 m_centerPos;

	//!@brief		回転した量
	DirectX::XMFLOAT3 m_angle;

	//!@brief		大きさ
	DirectX::XMFLOAT3 m_size; //大きさ 3Dのxfile用

	//!@brief		加速度
	//!@details		以前の座標からの移動量
	DirectX::XMFLOAT3 m_vector;

	/**
	 * @brief 前回回転した量
	 */
	DirectX::XMFLOAT3 m_lastAngle = {0, 0, 0};

public:
	//!@brief		行列
	DirectX::XMFLOAT4X4 m_matrix;

	/*!
	@brief		コンストラクタ
	@param	x	xいれる
	@param	y	yいれる
	@param	z	zいれる
	*/
	CMatrix(float x, float y, float z);

	//!@brief		移譲コンストラクタ1
	CMatrix() : CMatrix(0, 0, 0){};

	//!@brief		移譲コンストラクタ2
	//!@param	pos	位置入れる
	CMatrix(DirectX::XMFLOAT3 pos) : CMatrix(m_centerPos.x, m_centerPos.y, m_centerPos.z){};

	//!@brief		デストラクタ
	~CMatrix();

	//!@brief		毎フレーム動く処理突っ込む！
	void Update();

	//!@brief		ワールド座標ををセットする関数1
	void SetPos(float x, float y, float z)
	{
		m_centerPos = {x, y, z};
	}

	//!@brief		ワールド座標ををセットする関数2
	void SetPos(DirectX::XMFLOAT3 pos)
	{
		m_centerPos = pos;
	}

	//!@brief		角度をセットする関数1
	void SetAngle(float x, float y, float z)
	{
		m_angle = {x, y, z};
	}

	//!@brief		角度をセットする関数2
	void SetAngle(DirectX::XMFLOAT3 angle)
	{
		m_angle = angle;
	}

	/**
	 * @brief 	大きさをセットする関数１
	 * @param x x方向にどれくらい大きくしたいか
	 * @param y y方向にどれくらい大きくしたいか
	 * @param z z方向にどれくらい大きくしたいか
	 */
	void SetSize(float x, float y, float z)
	{
		m_size = {x, y, z};
	}

	/**
	 * @brief 		大きさをセットする関数２
	 * @param size 	セットしたい大きさ
	 */
	void SetSize(DirectX::XMFLOAT3 size)
	{
		m_size = size;
	}

	/**
	 * @brief 	移動したい量
	 * @param x ローカル座標のx方向に移動したい量
	 * @param y ローカル座標のy方向に移動したい量
	 * @param z ローカル座標のz方向に移動したい量
	 */
	void SetVec(float x, float y, float z)
	{
		m_vector = {x, y, z};
	}

	/**
	 * @brief 		移動したい量２
	 * @param vec 	ローカル座標の方向で移動したい量をいれる
	 */
	void SetVec(DirectX::XMFLOAT3 vec)
	{
		m_vector = vec;
	}

	/**
	 * @brief 			CMatrixのクラスをセットするメンバ
	 * @param matrix 	セットしたいCMatrix
	 */
	void SetCMatrix(const CMatrix &matrix);

	/**
	 * @brief 			行列のみをセットするクラス
	 * @details			SetPosで座標をセットする必要あり
	 * @param matrix 	セットしたい行列を入れる
	 */
	void SetMatrix(const DirectX::XMFLOAT4X4 &matrix)
	{
		m_matrix = matrix;
	}

	/**
	 * @brief						行列を取得する関数
	 * @return DirectX::XMFLOAT4X4 	取得したい行列
	 */
	[[nodiscard]] const DirectX::XMFLOAT4X4 &GetMatrix()
	{
		return m_matrix;
	}

	/**
	 * @brief 						ワールド座標取得メンバ関数
	 * @return DirectX::XMFLOAT3 	取得したいワールド座標
	 */
	[[nodiscard]] DirectX::XMFLOAT3 GetWorldPos()
	{
		return m_centerPos;
	}
};