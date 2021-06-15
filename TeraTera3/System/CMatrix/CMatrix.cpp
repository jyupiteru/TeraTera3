/**
 * @file CMatrix.cpp
 * @author jupiter
 * @brief 	CMatrixの処理が書かれているcpp
 * @version 1.1
 * @date 2020-07-19
 */

#include "CMatrix.h"

using namespace DirectX;

CMatrix::CMatrix(float x, float y, float z)
{
	m_centerPos = {x, y, z};

	m_angle = {0, 0, 0};

	m_size = {1, 1, 1};

	m_vector = {0, 0, 0};

	DX11MtxIdentity(m_matrix); //単位行列化　つまり初期化
}

CMatrix::~CMatrix()
{
}

void CMatrix::Update()
{
	//行列は拡大、回転、移動の順番！
	//DX11MtxIdentity(m_matrix);

	XMFLOAT4 localVectorX; //ローカル（モデル等）のXベクトルがワールドでどうなっているかが入る
	XMFLOAT4 localVectorY; //ローカル（モデル等）のYベクトルがワールドでどうなっているかが入る
	XMFLOAT4 localVectorZ; //ローカル（モデル等）のZベクトルがワールドでどうなっているかが入る

	XMFLOAT4X4 size;
	{
		//大きさ一度元に戻す
		auto size_x = m_matrix._11 * m_matrix._11 + m_matrix._12 * m_matrix._12 + m_matrix._13 * m_matrix._13;
		auto size_y = m_matrix._21 * m_matrix._21 + m_matrix._22 * m_matrix._22 + m_matrix._23 * m_matrix._23;
		auto size_z = m_matrix._31 * m_matrix._31 + m_matrix._32 * m_matrix._32 + m_matrix._33 * m_matrix._33;
		DX11MtxScale(1 / sqrt(size_x), sqrt(1 / size_y), sqrt(1 / size_z), size);
		DX11MtxMultiply(m_matrix, size, m_matrix);
	}
	{ //拡大処理
		DX11MtxScale(m_size.x, m_size.y, m_size.z, size);
	}

	{ //回転の処理

		//ローカルのXベクトル抜き出し作業
		localVectorX.x = m_matrix._11;
		localVectorX.y = m_matrix._12;
		localVectorX.z = m_matrix._13;
		localVectorX.w = 0.0f;

		//ローカルのYベクトル抜き出し作業
		localVectorY.x = m_matrix._21;
		localVectorY.y = m_matrix._22;
		localVectorY.z = m_matrix._23;
		localVectorY.w = 0.0f;

		//ローカルのZベクトル抜き出し作業
		localVectorZ.x = m_matrix._31;
		localVectorZ.y = m_matrix._32;
		localVectorZ.z = m_matrix._33;
		localVectorZ.w = 0.0f;

		//クオータニオン生成

		XMFLOAT4 quaternion;

		//行列からクォータニオンを生成する
		DX11GetQtfromMatrix(m_matrix, quaternion);

		XMFLOAT4 qtx, qty, qtz;

		//各軸を中心に回転したクォータニオン生成
		DX11QtRotationAxis(qtx, localVectorX, m_lastAngle.x - m_angle.x);
		DX11QtRotationAxis(qty, localVectorY, m_lastAngle.y - m_angle.y);
		DX11QtRotationAxis(qtz, localVectorZ, m_lastAngle.z - m_angle.z);

		//クォータニオンを合成
		XMFLOAT4 i1;
		XMFLOAT4 i2;
		XMFLOAT4 i3;

		DX11QtMul(i1, quaternion, qtx);

		DX11QtMul(i2, qty, qtz);

		DX11QtMul(i3, i1, i2);
		//クォータニオンをノーマライズ
		DX11QtNormalize(i3, i3);

		//クォータニオンから行列を生成
		DX11MtxFromQt(m_matrix, i3);
	}

	{ //移動量追加

		//ローカルのx方向への移動量追加
		m_centerPos.x += m_matrix._11 * m_vector.x;
		m_centerPos.y += m_matrix._12 * m_vector.x;
		m_centerPos.z += m_matrix._13 * m_vector.x;

		//ローカルのx方向への移動量追加
		m_centerPos.x += m_matrix._21 * m_vector.y;
		m_centerPos.y += m_matrix._22 * m_vector.y;
		m_centerPos.z += m_matrix._23 * m_vector.y;

		//ローカルのx方向への移動量追加
		m_centerPos.x += m_matrix._31 * m_vector.z;
		m_centerPos.y += m_matrix._32 * m_vector.z;
		m_centerPos.z += m_matrix._33 * m_vector.z;

		//現在地の追加
		m_matrix._41 = m_centerPos.x;
		m_matrix._42 = m_centerPos.y;
		m_matrix._43 = m_centerPos.z;

		m_lastAngle = m_angle;
	}
	DX11MtxMultiply(m_matrix, size, m_matrix);
}

void CMatrix::SetCMatrix(const CMatrix& matrix)
{
	m_angle = matrix.m_angle;
	m_size = matrix.m_size;
	m_vector = matrix.m_vector;
	m_centerPos = matrix.m_centerPos;
	m_matrix = matrix.m_matrix;
	Update();
}
