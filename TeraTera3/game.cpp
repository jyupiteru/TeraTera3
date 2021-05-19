/**
 * @file 	game.cpp
 * @author jupiter
 * @brief 	ゲームの本体ループ処理等記述しているcpp
 * @version 1.0
 * @date 	2020-07-11
 */
#include "game.h"

#include "Timer/CTimer.h"

using namespace DirectX;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

std::unique_ptr<CTimer> g_pTimer;

void GameMain(float fps)
{
	GameInput();	 // 入力
	GameUpdate(fps); // 更新
	GameDraw();		 // 描画
}

//================================================================================================
//================================================================================================

bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	{
		g_pTimer = std::make_unique<CTimer>();
	}

	bool sts;

	// DX11初期処理
	sts = DX11Init(hwnd, width, height, fullscreen);
	if (!sts)
	{
		MessageBox(hwnd, TEXT("DX11 init error"), TEXT("error"), MB_OK);
		return false;
	}

	//SetTransformの初期化
	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts)
	{
		MessageBox(NULL, TEXT("SetTransform error"), TEXT("Error"), MB_OK);
		return false;
	}

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);

	// アルファブレンド有効にする
	TurnOnAlphablend();

	return true;
}

//================================================================================================
//================================================================================================

void GameInput()
{
	// キー入力取得
	CDirectInput::GetInstance().GetKeyBuffer();

	//マウスの状態を取得
	CDirectInput::GetInstance().GetMouseState();
}

//================================================================================================
//================================================================================================

void GameUpdate(float fps)
{
	g_pTimer->Update();
}

//================================================================================================
//================================================================================================

void GameDraw()
{
	// ターゲットバッファクリア
	float ClearColor[4] = {0.0f, 0.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// レンダリング前処理
	DX11BeforeRender(ClearColor);

	// レンダリング後処理
	DX11AfterRender();
}

//================================================================================================
//================================================================================================

void GameUninit()
{
	DX11SetTransform::GetInstance()->Uninit();
	g_pTimer.reset();

	DX11Uninit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
