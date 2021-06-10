#pragma once
#include <d3dcompiler.h>
#include <directxmath.h>
#include <locale.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "directXtex.h"

/**
 * @brief ファイル名を取得する処理
 * @param fullpath ファイルまでのフルのパス
 * @param split ??
 * @return std::string ファイル名
 */
std::string ExtractFileName(std::string fullpath, const char *split);

/**
 * @brief ファイルの拡張子を取得する処理
 * @param filename ファイル名
 * @return std::string 拡張子名
 */
std::string GetFileExt(const char *filename);

/**
 * @brief コンパイル済みシェーダーファイルを読み込む処理
 * @param csoName
 * @param byteArray
 * @return true 成功
 * @return false 失敗
 */
bool readShader(const char *csoName, std::vector<unsigned char> &byteArray);

/**
 * @brief シェーダーをファイル拡張子に合わせてコンパイル
 * @param szFileName
 * @param szEntryPoint
 * @param szShaderModel
 * @param ShaderObject
 * @param ShaderObjectSize
 * @param ppBlobOut
 * @return HRESULT
 */
HRESULT CompileShader(const char *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void **ShaderObject, size_t &ShaderObjectSize, ID3DBlob **ppBlobOut);

/**
 * @brief シェーダーをコンパイルする処理
 * @param szFileName
 * @param szEntryPoint
 * @param szShaderModel
 * @param ppBlobOut
 * @return HRESULT
 */
HRESULT CompileShaderFromFile(const char *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut);

/**
 * @brief 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成する処理
 * @param device デバイス
 * @param szFileName
 * @param szEntryPoint
 * @param szShaderModel
 * @param layout
 * @param numElements
 * @param ppVertexShader
 * @param ppVertexLayout
 * @return true 成功
 * @return false 失敗
 */
bool CreateVertexShader(ID3D11Device *device,
						const char *szFileName,
						LPCSTR szEntryPoint,
						LPCSTR szShaderModel,
						D3D11_INPUT_ELEMENT_DESC *layout,
						unsigned int numElements,
						ID3D11VertexShader **ppVertexShader,
						ID3D11InputLayout **ppVertexLayout);
/**
 * @brief ピクセルシェーダーオブジェクトを生成する処理
 * @param device デバイス
 * @param szFileName ファイル名
 * @param szEntryPoint
 * @param szShaderModel
 * @param ppPixelShader
 * @return true 成功
 * @return false 失敗
 */
bool CreatePixelShader(ID3D11Device *device, const char *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11PixelShader **ppPixelShader);

/**
 * @brief コンスタントバッファの作成
 * @param device DirectX11のデバイス
 * @param bytesize コンスタントバッファサイズ
 * @param pConstantBuffer  コンスタントバッファ
 * @return true 成功
 * @return false 失敗
 */
bool CreateConstantBuffer(ID3D11Device *device, unsigned int bytesize, ID3D11Buffer **pConstantBuffer);

/**
 * @brief Mapで書き換え可能なコンスタントバッファの作成
 * @param device デバイス
 * @param bytesize コンスタントバッファサイズ
 * @param pConstantBuffer コンスタントバッファ
 * @return true 成功
 * @return false 失敗
 */
bool CreateConstantBufferWrite(ID3D11Device *device, unsigned int bytesize, ID3D11Buffer **pConstantBuffer);

/**
 * @brief インデックスバッファの作成
 * @param device デバイス
 * @param indexnum 頂点数
 * @param indexdata インデックスデータ格納メモリ先頭アドレス
 * @param pIndexBuffer 生成先のバッファ？
 * @return true 成功
 * @return false 失敗
 */
bool CreateIndexBuffer(ID3D11Device *device, unsigned int indexnum, void *indexdata, ID3D11Buffer **pIndexBuffer);

/**
 * @brief Mapで書き換え可能なインデックスバッファの作成
 * @param device  デバイス
 * @param indexnum インデックス数
 * @param indexdata インデックスデータ格納メモリ先頭アドレス
 * @param pIndexBuffer インデックスバッファ
 * @return true 成功
 * @return false 失敗
 */
bool CreateIndexBufferWrite(ID3D11Device *device, unsigned int indexnum, void *indexdata, ID3D11Buffer **pIndexBuffer);

/**
 * @brief 頂点シェーダーオブジェクトを生成する処理
 * @param device DirectX11の
 * @param stride １頂点当たりバイト数
 * @param vertexnum 頂点数
 * @param vertexdata 頂点データ格納メモリ先頭アドレス
 * @param pVertexBuffer 頂点バッファ
 * @return true 成功
 * @return false 失敗
 */
bool CreateVertexBuffer(ID3D11Device *device, unsigned int stride, unsigned int vertexnum, void *vertexdata, ID3D11Buffer **pVertexBuffer);

/**
 * @brief CPU書き込み可能な頂点バッファの生成
 * @param device デバイス
 * @param stride １頂点当たりバイト数
 * @param vertexnum 頂点数
 * @param vertexdata 頂点データ格納メモリ先頭アドレス
 * @param pVertexBuffer 頂点バッファ
 * @return true 成功
 * @return false 失敗
 */
bool CreateVertexBufferWrite(ID3D11Device *device, unsigned int stride, unsigned int vertexnum, void *vertexdata, ID3D11Buffer **pVertexBuffer);

/**
 * @brief 頂点バッファ(UAV)を作成
 * @param device デバイス
 * @param stride １頂点当たりバイト数
 * @param vertexnum 頂点数
 * @param vertexdata 頂点データ格納メモリ先頭アドレス
 * @param pVertexBuffer 頂点バッファ
 * @return true 成功
 * @return false 失敗
 */
bool CreateVertexBufferUAV(ID3D11Device *device, unsigned int stride, unsigned int vertexnum, void *vertexdata, ID3D11Buffer **pVertexBuffer);

/**
 * @brief Structuredバッファを作成
 * @param device デバイス
 * @param stride ストライドバイト数
 * @param num 個数
 * @param data データ格納メモリ先頭アドレス
 * @param pStructuredBuffer RWStructuredBuffer
 * @return true 成功
 * @return false 失敗
 */
bool CreateStructuredBuffer(ID3D11Device *device, unsigned int stride, unsigned int num, void *data, ID3D11Buffer **pStructuredBuffer);

/**
 * @brief STAGINGバッファを作成しコピーする
 * @param device デバイス
 * @param devicecontext デバイスコンテキスト
 * @param pBuffer RWStructuredBuffer
 * @return ID3D11Buffer* ??
 */
ID3D11Buffer *CreateAndCopyToBuffer(ID3D11Device *device, ID3D11DeviceContext *devicecontext, ID3D11Buffer *pBuffer);

/**
 * @brief バッファからシェーダーリソースビューを作成する
 * @param device デバイス
 * @param pBuffer Buffer
 * @param ppSRV
 * @return true 成功
 * @return false 失敗
 */
bool CreateShaderResourceView(ID3D11Device *device, ID3D11Buffer *pBuffer, ID3D11ShaderResourceView **ppSRV);

/**
 * @brief UnOrderedAccessViewを作成
 * @param device デバイス
 * @param pBuffer Buffer
 * @param ppUAV
 * @return true 成功
 * @return false 失敗
 */
bool CreateUnOrderAccessView(ID3D11Device *device, ID3D11Buffer *pBuffer, ID3D11UnorderedAccessView **ppUAV);

/**
 * @brief ＴＧＡファイルを読み込みシェーダーリソースビュー―を作成する
 * @param filename ファイル名
 * @param device デバイス
 * @param resource
 * @param srv
 * @return true 成功
 * @return false 失敗
 */
bool CreateSRVfromTGAFile(const char *filename, ID3D11Device *device, ID3D11Resource **resource, ID3D11ShaderResourceView **srv);

/**
 * @brief WICファイルを読み込みシェーダーリソースビューを作成する
 * @param filename ファイル名
 * @param device デバイス
 * @param device11Context でアイスコンテキスト
 * @param resource
 * @param srv
 * @return true 成功
 * @return false 失敗
 */
bool CreateSRVfromWICFile(const char *filename,  ID3D11Device * device, ID3D11DeviceContext *device11Context, ID3D11Resource **resource, ID3D11ShaderResourceView **srv);

/**
 * @brief DDSファイルを読み込みシェーダーリソースビュー―を作成する
 * @param filename ファイル名
 * @param device デバイス
 * @param device11Context デバイスコンテキスト
 * @param resource
 * @param srv
 * @return true 成功
 * @return false 失敗
 */
bool CreateSRVfromDDS(const char *filename, ID3D11Device *device, ID3D11DeviceContext *device11Context, ID3D11Resource **resource, ID3D11ShaderResourceView **srv);

/**
 * @brief ファイルを読み込みシェーダーリソースビューを作成する
 * @param filename ファイル名
 * @param device デバイス
 * @param device11Context デバイスコンテキスト
 * @param resource
 * @param srv
 * @return true 成功
 * @return false 失敗
 */
bool CreateSRVfromFile(const char *filename, ID3D11Device *device, ID3D11DeviceContext *device11Context, ID3D11Resource **resource, ID3D11ShaderResourceView **srv);
