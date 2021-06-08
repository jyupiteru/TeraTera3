/**
 * @file    game.h
 * @author  jupiter
 * @brief   Gameのループ処理等記述する関数等を宣言するヘッダ
 */

#pragma once

#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <thread>
#include <DirectXMath.h>
#include <tchar.h>
#include <memory>

#include "Setup.h"

#include "WindowsSystem/CDirectInput.h"
#include "WindowsSystem/DX11Settransform.h"
#include "WindowsSystem/dx11mathutil/Dx11mathutil.h"

//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

#include "../ThirdParty/ImGui/imgui.h"
#include "../ThirdParty/ImGui/imgui_impl_dx11.h"
#include "../ThirdParty/ImGui/imgui_impl_win32.h"

/**
 * @brief
 */
void GameMain(float fps);

//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);

/**
 * @brief   Gameの廃棄処理
 */
void GameUninit();

/**
 * @brief   Gameのキー入力情報取得処理
 */
void GameInput();

/**
 * @brief   Gameのアップデート処理
 */
void GameUpdate(float fps);

/**
 * @brief   Gameの描画処理
 */
void GameDraw();
