/**
 * @file 	game.cpp
 * @author jupiter
 * @brief 	ゲームの本体ループ処理等記述しているcpp
 * @version 1.0
 * @date 	2020-07-11
 */
#include "game.h"

#include "ComSystem/Core/ObjectGenerator.h"
#include "Comsystem/Core/GameObject.h"

#include "Comsystem/ComSystem.h"
#include "DebugLog/DebugLog.h"

#include "SceneSystem/CSceneManager/CSceneManager.h"
#include "ComSystem/EventSystem/EventSystem.h"
#include "Container/CContainer.h"
#include "ImGuiManager/CImGuiManager.h"
#include "ImGuiManager/CImGuiWindow.h"

using namespace DirectX;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//各種staticで破棄を管理したいもの
std::unique_ptr<CSceneManager> g_pSceneManager;
std::unique_ptr<ObjectGenerator> g_pObjectGenerator;
std::unique_ptr<Timer> g_pTimer;
std::unique_ptr<EventSystem> g_pEventSystem;
std::unique_ptr<DebugLog> g_pDebugLog;
std::unique_ptr<CContainer> g_pContainer;
std::unique_ptr<CImGuiManager> g_pImGuiManager;

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
	//リソースを管理するコンテナクラスの作成
	g_pContainer = std::make_unique<CContainer>();

	//タイマー関数初期化
	g_pTimer = std::make_unique<Timer>();

	//デバッグ用のログの初期化
	g_pDebugLog = std::make_unique<DebugLog>();

#ifdef _DEBUG

	DebugLog::Draw("==========DirectX Template is Start by Debug==========\n\n");

	DebugLog::Draw("author:jupiter\n");

	DebugLog::Draw("Start GameInit()\n");

	std::string screecstr = "SCREENWIDTH:" + std::to_string(SCREEN_WIDTH);
	DebugLog::Draw(screecstr);

	screecstr = "SCREENHEIGHT:" + std::to_string(SCREEN_HEIGHT);
	DebugLog::Draw(screecstr);

#endif

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

	//ImGUiのマネージャーの起動
	g_pImGuiManager = std::make_unique<CImGuiManager>();
	g_pImGuiManager->Init(hwnd);

	//ウインドウを１個生成
	unsigned int windowid = g_pImGuiManager->CreateImGuiWindow();
	auto windowdata = CImGuiManager::GetImGuiWindow(windowid);

	//大きさを1/3にしたいので計算
	int size_ = SCREEN_WIDTH / 3;
	int sizecount = 0;
	while (sizecount * 50 < size_)
	{
		sizecount++;
	}

	//ウインドウの座標を変更
	windowdata->m_firstSize.x = static_cast<float>(sizecount * 50);
	windowdata->m_firstSize.y = static_cast<float>(SCREEN_HEIGHT);

	windowdata->m_firstCenterPosition.x = static_cast<float>(SCREEN_WIDTH) - windowdata->m_firstSize.x / 2.0f;
	windowdata->m_firstCenterPosition.y = windowdata->m_firstSize.y / 2.0f;

	//オブジェクト管理リスト生成
	g_pObjectGenerator = std::make_unique<ObjectGenerator>();

	g_pImGuiManager->SetImGuiFunction("ObjectList", g_pObjectGenerator.get(), "Menu");
	g_pImGuiManager->SetImGuiFunction("Objects", std::bind(&ObjectGenerator::ImGuiDraw_Objects, g_pObjectGenerator.get(), std::placeholders::_1), "Menu");

	//イベントシステム初期化
	g_pEventSystem = std::make_unique<EventSystem>();

	// カメラ変換行列初期化
	XMFLOAT3 eye = {0, 0, -30};	 // 視点
	XMFLOAT3 lookat = {0, 0, 0}; // 注視点
	XMFLOAT3 up = {0, 1, 0};	 // 上向きベクトル

	//カメラオブジェクトを生成
	auto camera = GameObject::MakeNewObject("camera", E_TYPE_OBJECT::NONE);
	camera->AddComponent<ComCamera>()->SetCamera(eye, lookat, up);

	camera->GetComponent<ComCamera>()->SetProjection(1.0f, 100000.0f,
													 XM_PIDIV2, SCREEN_WIDTH, SCREEN_HEIGHT);
	camera->DontDestroyOnLoad();
	camera->m_objectUpdatePriority.SetValue(-20);

	// 平行光源初期化
	DirectX::XMFLOAT4 lightpos = {1, 1, -1, 0}; // 平行光源の方向をセット

	//シーンに設置するライトを生成
	auto light = GameObject::MakeNewObject("light", E_TYPE_OBJECT::NONE);
	light->GetComponent<ComTransform>()->m_worldPosition.SetValue(lightpos.x,
																  lightpos.y,
																  lightpos.z);

	light->AddComponent<ComLight>()->m_ambient.SetValue(0.0f,
														0.0f,
														0.0f,
														0.0f); // 環境光

	light->m_objectUpdatePriority.SetValue(-20);
	light->DontDestroyOnLoad();

	// アルファブレンド有効にする
	TurnOnAlphablend();

	g_pSceneManager = std::make_unique<CSceneManager>();

	g_pImGuiManager->SetImGuiFunction("SceneList", g_pSceneManager.get(), "Menu");
	g_pImGuiManager->SetImGuiFunction("EventSystem", g_pEventSystem.get(), "Menu");

#ifdef _DEBUG
	DebugLog::Draw("End GameInit()\n");
#endif

	g_pTimer->Update();

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
#ifdef _DEBUG
	g_pDebugLog->Update();
#endif

	g_pTimer->Update();

	g_pEventSystem->Update();

	//シーンに存在しているオブジェクトのUpdateをぶん回し
	g_pSceneManager->Update();
}

//================================================================================================
//================================================================================================

void GameDraw()
{
	// ターゲットバッファクリア
	float ClearColor[4] = {0.0f, 0.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// レンダリング前処理
	DX11BeforeRender(ClearColor);

	//シーンに存在しているオブジェクトのDrawをぶん回し
	g_pSceneManager->Draw();

	g_pImGuiManager->Draw();

	// レンダリング後処理
	DX11AfterRender();
}

//================================================================================================
//================================================================================================

void GameUninit()
{
	DX11SetTransform::GetInstance()->Uninit();

	DX11Uninit();

	g_pDebugLog.reset();
	g_pImGuiManager.reset();
	g_pObjectGenerator.reset();
	g_pSceneManager.reset();
	g_pEventSystem.reset();
	g_pTimer.reset();
	g_pContainer.reset();
}

//******************************************************************************
//	End of file.
//******************************************************************************
