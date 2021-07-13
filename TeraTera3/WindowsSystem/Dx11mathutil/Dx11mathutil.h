/**
 * @file 	Dx11mathutil.h
 * @brief 	計算用の関数が集められたヘッダ
 */

#pragma once
#include <DirectXMath.h>
#include <assimp\Importer.hpp>

#define ALIGN16 _declspec(align(16))
struct tagPLANE
{
	float a, b, c, d;
};

/**
 * @brief ワールド変換行列を作成する関数
 * @param mat 行列を格納する変数
 * @param angle 角度
 * @param trans 大きさ
 */
void DX11MakeWorldMatrix(DirectX::XMFLOAT4X4 &mat, DirectX::XMFLOAT3 &angle, DirectX::XMFLOAT3 &trans);

/**
 * @brief 3次元ベクトルと行列をかける処理
 * @param outvec 出力先の変数
 * @param invec かけたいベクトル
 * @param mat かけたい行列
 */
void DX11Vec3MulMatrix(DirectX::XMFLOAT3 &outvec, const DirectX::XMFLOAT3 &invec, const DirectX::XMFLOAT4X4 &mat);

/**
 * @brief 外積を計算する関数
 * @param outvec 出力先の変数
 * @param invec1 外積を求めたいベクトル1
 * @param invec2 外積を求めたいベクトル2
 */
void DX11Vec3Cross(DirectX::XMFLOAT3 &outvec, DirectX::XMFLOAT3 &invec1, DirectX::XMFLOAT3 &invec2);

/**
 * @brief 内積を計算する関数
 * @param dot 出力先の変数
 * @param invec1 内積を求めたいベクトル1
 * @param invec2 内積を求めたいベクトル2
 */
void DX11Vec3Dot(float &dot, const DirectX::XMFLOAT3 &invec1, const DirectX::XMFLOAT3 &invec2);

/**
 * @brief ベクトルを正規化する関数
 * @param outvec 出力先の変数
 * @param invec 内積したいベクトル
 */
void DX11Vec3Normalize(DirectX::XMFLOAT3 &outvec, DirectX::XMFLOAT3 &invec);

/**
 * @brief 単位行列にする関数
 * @param mat 単位行列にしたい行列
 */
void DX11MtxIdentity(DirectX::XMFLOAT4X4 &mat);

/**
 * @brief 行列からクォータニオンを生成する関数
 * @param mat クォータニオンを取り出したい行列
 * @param qt 出力先の変数
 */
void DX11GetQtfromMatrix(const DirectX::XMFLOAT4X4 &mat, DirectX::XMFLOAT4 &qt);

/**
 * @brief 指定軸を中心に回転したクォータニオンを生成する関数
 * @param qt 出力先変数
 * @param axis 指定軸
 * @param angle 回転する角度
 */
void DX11QtRotationAxis(DirectX::XMFLOAT4 &qt, DirectX::XMFLOAT4 &axis, float angle);

/**
 * @brief クォータニオン同士を掛け算する処理
 * @param outqt 出力先変数
 * @param qt1 かけたいクォータニオン1
 * @param qt2 かけたいクォータニオン2
 */
void DX11QtMul(DirectX::XMFLOAT4 &outqt, const DirectX::XMFLOAT4 &qt1, const DirectX::XMFLOAT4 &qt2);

/**
 * @brief クォータニオンから行列を作成する処理
 * @param outmtx 出力先変数
 * @param qt 生成元のクォータニオン
 */
void DX11MtxFromQt(DirectX::XMFLOAT4X4 &outmtx, const DirectX::XMFLOAT4 &qt);

/**
 * @brief 転地する処理
 * @param outmtx 出力先変数
 * @param inmtx 転地したい値
 */
void DX11MtxTranspose(DirectX::XMFLOAT4X4 &outmtx, const DirectX::XMFLOAT4X4 &inmtx);

/**
 * @brief クォータニオンの単位ベクトル化をおこなう処理
 * @param inqt 単位ベクトル化したいクォータニオン
 * @param outqt 出力先変数
 */
void DX11QtNormalize(DirectX::XMFLOAT4 &inqt, DirectX::XMFLOAT4 &outqt);

/**
 * @brief ベクトルの長さを求める処理1
 * @param vec 求めたいベクトル
 * @param length 出力先変数
 */
void DX11Vec4Length(DirectX::XMFLOAT4 &vec, float &length);

/**
 * @brief ベクトルの長さを求める処理1
 * @param vec 求めたいベクトル
 * @param length 出力先変数
 */
void DX11Vec3Length(DirectX::XMFLOAT3 &vec, float &length);

/**
 * @brief 平行移動行列を生成する処理
 * @param trans 座標情報
 * @param matrix 出力先の変数
 */
void DX11MtxTranslation(const DirectX::XMFLOAT3 &trans, DirectX::XMFLOAT4X4 &matrix);

/**
 * @brief 単位クォータニオンを生成する関数
 * @param qt 単位化したいクォータニオン
 */
void DX11QtIdentity(DirectX::XMFLOAT4 &qt);

/**
 * @brief 指定軸で回転する行列を作成する関数
 * @param axis 指定軸
 * @param angle 回転の角度(度数法)
 * @param outmtx 出力先の変数
 */
void DX11MtxRotationAxis(const DirectX::XMFLOAT3 &axis, float angle, DirectX::XMFLOAT4X4 &outmtx);

/**
 * @brief 行列の掛け算
 * @param ansmtx 出力先の変数
 * @param p1mtx かけたい行列1
 * @param p2mtx かけたい行列2
 */
void DX11MtxMultiply(DirectX::XMFLOAT4X4 &ansmtx, const DirectX::XMFLOAT4X4 &p1mtx, const DirectX::XMFLOAT4X4 &p2mtx);

/**
 * @brief 逆行列を求める関数
 * @param ansmtx 出力先変数
 * @param mtx 逆行列を求めたい行列
 */
void DX11MtxInverse(DirectX::XMFLOAT4X4 &ansmtx, const DirectX::XMFLOAT4X4 &mtx);

/**
 * @brief フラスタム内かどうかをチェックをする関数
 * @param pos 位置座標（ワールド）
 * @param matrix ビュー変換行列＊プロジェクション変換行列
 * @return true 内にいる
 * @return false いない
 */
bool IsInFrustum(const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT4X4 &matrix);

/**
 * @brief フラスタムないかチェックする関数2
 * @param pos 位置座標（ワールド）
 * @param matrix ビュー変換行列＊プロジェクション変換行列
 * @param ans ６平面との距離
 */
void IsInFrustum(const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT4X4 &matrix, float ans[]);

/**
 * @brief フラスタムないかチェックする関数3
 * @param pos 位置座標（ワールド）
 * @param matrix ビュー変換行列＊プロジェクション変換行列
 * @param ans 6面との距離
 * @param plane 
 * @return true フラスタム内
 * @return false フラスタム外
 */
bool IsInFrustum(const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT4X4 &matrix, float ans[], tagPLANE *plane);

/**
 * @brief  フラスタムないかチェックする関数4
 * @param pos 位置座標（ワールド）
 * @param radius ＢＳ半径
 * @param matrix ビュー変換行列＊プロジェクション変換行列
 * @param ans フラスタム平面との距離
 * @param canseeflag ＢＳが見えるかどうかフラグ
 * @param plane 平面の方程式
 * @return true フラスタム内
 * @return false フラスタム外
 */
bool IsInFrustum(const XMFLOAT3 &pos, float radius, const XMFLOAT4X4 &matrix, float ans[], bool canseeflag[], tagPLANE *plane); // 平面方程式;

/**
 * @brief プロジェクション変換行列からフラスタムの６平面を取り出す
 * @param p 平面の方程式
 * @param matrix ビュー変換行列＊プロジェクション変換行列
 */
void GetPlanefromProjectionmatrix(tagPLANE *p, const DirectX::XMFLOAT4X4 &matrix);

/**
 * @brief プロジェクション変換行列からフラスタムの６平面を取り出す(正規化行う)
 * @param p 平面の方程式
 * @param matrix ビュー変換行列＊プロジェクション変換行列
 */
void GetPlanefromProjectionmatrixWithNormalize(tagPLANE *p, const DirectX::XMFLOAT4X4 &matrix);

/**
 * @brief X軸回転行列を求める
 * @param angle 回転角度
 * @param outmtx 出力先変数
 */
void DX11MtxRotationX(float angle, DirectX::XMFLOAT4X4 &outmtx);

/**
 * @brief Y軸回転行列を求める
 * @param angle 回転角度
 * @param outmtx 出力先変数
 */
void DX11MtxRotationY(float angle, DirectX::XMFLOAT4X4 &outmtx);

/**
 * @brief Z軸回転行列を求める
 * @param angle 回転角度
 * @param outmtx 出力先変数
 */
void DX11MtxRotationZ(float angle, DirectX::XMFLOAT4X4 &outmtx);

/**
 * @brief 拡大行列を求める
 * @param sx Xの大きさ
 * @param sy Yの大きさ
 * @param sz Zの大きさ
 * @param outmtx 出力先変数
 */
void DX11MtxScale(float sx, float sy, float sz, DirectX::XMFLOAT4X4 &outmtx);

/**
 * @brief ハルトン数列を計算
 * @param no 何番目か
 * @param base 基底?
 * @return float ハルトン数列値
 */
float haltonseq(unsigned int no, unsigned int base);

/**
 * @brief Assimo用の行列からDirectX用の行列へ変換する処理
 * @param aimtx Assimpの行列
 * @return DirectX::XMFLOAT4X4 DirectXの行列
 */
DirectX::XMFLOAT4X4 DX11MtxaiToDX(aiMatrix4x4 &aimtx);