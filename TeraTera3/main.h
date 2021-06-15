/**
 * @file    main.h
 * @author  jupiter
 * @brief   すべての元のファイル
 * @version 1.0
 * @date    2020-07-11
 */

#pragma once

#include <memory>

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <thread>
#include <tchar.h>

#include "game.h"
#include "Setup.h"

#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)  // ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2) // ウインドウの中心Ｙ座標

#pragma comment(lib, "d3d11.lib")            // リンク対象ライブラリにd3d11.libを追加
#pragma comment(lib, "dxgi.lib")             // リンク対象ライブラリにdxgxi.libを追加
#pragma comment(lib, "D3DCompiler.lib")      // リンク対象ライブラリにd3dcompiler.libを追加
#pragma comment(lib, "winmm.lib")            // リンク対象ライブラリにwinmm.libを追加
#pragma comment(lib, "directxtex.lib")       // リンク対象ライブラリにwinmm.libを追加
#pragma comment(lib, "dxguid.lib")           // リンク対象ライブラリにdxguid.libを追加
#pragma comment(lib, "dinput8.lib")          // リンク対象ライブラリにdinput8.libを追加
#pragma comment(lib, "assimp-vc141-mtd.lib") // リンク対象ライブラリにassimp-vc141-mtd.libを追加

//!	@fn		WinMain
//!	@brief	エントリポイント
//!	@param	hWnd	インスタンスハンドル
//!	@param	uMsg	意味なし
//!	@param	wParam	起動時の引数文字列
//!	@param	lParam	ウインドウ表示モード
//!	@retval	TRUE    成功終了
//! @retval	FALSE   失敗終了
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
 * @brief           ImgUiで操作を受け付けるのに使用する際に必要な宣言
 * @details         内容はWndProcと同じ
 * @param hwnd      インスタンスハンドル
 * @param msg       意味なし
 * @param wParam    起動時の引数文字列
 * @param lParam    ウインドウ表示モード
 * @return IMGUI_API    ？
 */
IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
