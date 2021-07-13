/**
 * @file main.cpp
 * @author jupiter
 * @brief メイン処理
 */

#include "main.h"

float g_fps[] = {0, 0, 0}; // ＦＰＳへのカウンタ

int g_fpscount = 0;

int APIENTRY WinMain(HINSTANCE hInstance,	  // アプリケーションのハンドル
					 HINSTANCE hPrevInstance, // 今のWindowsバージョンでは使われない
					 LPSTR lpszArgs,		  // 起動時の引数（文字列）
					 int nWinMode)			  // ウインドウ表示モード
{
	HWND hwnd;					// ウインドウハンドル
	MSG msg;					// メッセージ構造体
	WNDCLASSEX wcex;			// ウインドウクラス構造体
	int width = SCREEN_WIDTH;	// ウインドウの幅 計算用ワーク
	int height = SCREEN_HEIGHT; // ウインドウの高さ 計算用ワーク

	DWORD dwExecLastTime; // 最後に実行した時間
	DWORD dwFPSLastTime;  // 最後に計測した時間
	DWORD dwCurrentTime;  // 現在時刻
	DWORD dwFrameCount;	  // フレーム数

	// メモリリークを検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ウインドウクラス情報のセット
	wcex.hInstance = hInstance;								 // インスタンス値のセット
	wcex.lpszClassName = CLASS_NAME;						 // クラス名
	wcex.lpfnWndProc = (WNDPROC)WndProc;					 // ウインドウメッセージ関数
	wcex.style = 0;											 // ウインドウスタイル
	wcex.cbSize = sizeof(WNDCLASSEX);						 // 構造体のサイズ
	wcex.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); // ラージアイコン
	wcex.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);	 // スモールアイコン
	wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);	 // カーソルスタイル
	wcex.lpszMenuName = 0;									 // メニューなし
	wcex.cbClsExtra = 0;									 // エキストラなし
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 背景色白
	if (!RegisterClassEx(&wcex))
		return FALSE; // ウインドウクラスの登録

	if (FULLSCREEN)
	{
		hwnd = CreateWindow(
			CLASS_NAME,					 // ウィンドウクラスの名前
			WINDOW_NAME,				 // タイトル
			WS_VISIBLE | WS_POPUP,		 // ウィンドウスタイル
			0, 0,						 // ウィンドウ位置 縦, 横
			SCREEN_WIDTH, SCREEN_HEIGHT, // ウィンドウサイズ
			NULL,						 // 親ウィンドウなし
			(HMENU) nullptr,			 // メニューなし
			hInstance,					 // インスタンスハンドル
			(LPVOID) nullptr);			 // 追加引数なし
	}
	else
	{
		//タイトルバーとウインドウ枠の分を含めてウインドウサイズを設定
		RECT rect;
		SetRect(&rect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		int window_width = rect.right - rect.left;
		int window_height = rect.bottom - rect.top;
		rect.top = 0;
		rect.left = 0;

		hwnd = CreateWindowEx(0,				   // 拡張ウィンドウスタイル
							  CLASS_NAME,		   // ウィンドウクラスの名前
							  WINDOW_NAME,		   // タイトル
							  WS_OVERLAPPEDWINDOW, // ウィンドウスタイル
							  CW_USEDEFAULT,	   // ウィンドウの左上Ｘ座標
							  CW_USEDEFAULT,	   // ウィンドウの左上Ｙ座標
							  window_width,		   // ウィンドウの幅
							  window_height,	   // ウィンドウの高さ
							  nullptr,			   // 親ウィンドウのハンドル
							  nullptr,			   // メニューハンドルまたは子ウィンドウID
							  hInstance,		   // インスタンスハンドル
							  nullptr);			   // ウィンドウ作成データ
	}

	if (!hwnd)
		return FALSE;

	timeBeginPeriod(1); // タイマの分解能力を１msにする
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;
	// ウインドウを表示する
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	// ゲームの初期処理
	if (!GameInit(hInstance, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN))
	{
		GameUninit();
		MessageBox(hwnd, TEXT("ERROR!"), TEXT("GameInit Error"), MB_OK);
		return false;
	}

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) // メッセージを取得しなかった場合"0"を返す
		{												   // Windowsの処理
			if (msg.message == WM_QUIT)
			{ // PostQuitMessage()が呼ばれて、WM_QUITメッセージが来たらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ // DirectXの処理
			// ＦＰＳ一定化処理

			// 現在時間を取得
			dwCurrentTime = timeGetTime();
			// 0.5秒ごとに実行
			if ((dwCurrentTime - dwFPSLastTime) > 500)
			{

				// FPSを計算
				g_fps[g_fpscount] = static_cast<float>(dwFrameCount * 1000);
				g_fps[g_fpscount] /= (dwCurrentTime - dwFPSLastTime);

				if (sizeof(g_fps) / sizeof(float) <= g_fpscount)
				{
					g_fpscount = 0;
				}

				// ＦＰＳ計測時間に現座時間をセット
				dwFPSLastTime = dwCurrentTime;

				// フレーム数をゼロクリア
				dwFrameCount = 0;
			}

			// 1000ミリ秒(1秒)分のの設定フレーム時間秒経過したら
			if ((dwCurrentTime - dwExecLastTime) >= (1000.0f / MAX_FRAME_RATE))
			{

				// 最終実行時間に現在時間をセット
				dwExecLastTime = dwCurrentTime;

				GameMain(g_fps[0]);

				// フレーム数をインクリメント
				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1); // タイマの分解能力もとに戻す

	GameUninit();
	return (int)msg.wParam;
}

//================================================================================================
//================================================================================================

LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//ImGuiの操作を受け付けるのに必要な処理
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
