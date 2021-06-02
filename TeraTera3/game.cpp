/**
 * @file 	game.cpp
 * @author jupiter
 * @brief 	ゲームの本体ループ処理等記述しているcpp
 * @version 1.0
 * @date 	2020-07-11
 */
#include "game.h"

#include "Timer/CTimer.h"
#include "ImGuiSystem/CImGuiManager/CImGuiManager.h"
#include "EventSystem/CEventSystem.h"
#include "DebugLog/CDebugLog.h"
#include "ComSystem/ComSystem.h"
#include "ComSystem/Core/ObjectGenerator.h"
#include "SceneSystem/CSceneManager/CSceneManager.h"
#include "ResourceContainer/CContainer.h"

using namespace DirectX;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

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
	//タイマーを初期化
	CTimer::Create();
	CTimer::GetInstance().Update();

	//ログを起動
	CDebugLog::Create();

	CContainer::Create();

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

	//ImGuiを管理するかマネージャーの生成と初期化
	CImGuiManager::Create();
	CImGuiManager::GetInstance().Init(hwnd);

	CEventSystem::Create();

	ObjectGenerator::Create();

	CImGuiManager::GetInstance().SetImGuiFunction("ObjectList", &ObjectGenerator::GetInstance(), "Menu");
	CImGuiManager::GetInstance().SetImGuiFunction("Objects", std::bind(&ObjectGenerator::ImGuiDraw_Objects, &ObjectGenerator::GetInstance(), std::placeholders::_1), "Menu");

	{
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
	}

	CSceneManager::Create();

	CImGuiManager::GetInstance().SetImGuiFunction("SceneList", &CSceneManager::GetInstance(), "Menu");
	CImGuiManager::GetInstance().SetImGuiFunction("EventSystem", &CSceneManager::GetInstance(), "Menu");

	//ウインドウを1つ生成
	unsigned int windowid = CImGuiManager::GetInstance().CreateImGuiWindow();
	auto windowdata = CImGuiManager::GetInstance().GetImGuiWindow(windowid);

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
	//タイマーを更新する
	CTimer::GetInstance().Update();

	//3Dの衝突判定の更新
	CCollision3DSystem::GetInstance().Update();

	//シーンに存在しているオブジェクトのUpdateをぶん回し
	CSceneManager::GetInstance().Update();
}

//================================================================================================
//================================================================================================

void GameDraw()
{
	// ターゲットバッファクリア
	float ClearColor[4] = {0.0f, 0.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// レンダリング前処理
	DX11BeforeRender(ClearColor);

	CSceneManager::GetInstance().Draw();

	CImGuiManager::GetInstance().Draw();

	// レンダリング後処理
	DX11AfterRender();
}

//================================================================================================
//================================================================================================

void GameUninit()
{
	//各種確保したポインタの解放
	DX11SetTransform::GetInstance()->Uninit();
	CTimer::Delete(true);
	CDebugLog::Delete(true);
	ObjectGenerator::Delete(true);
	CSceneManager::Delete(true);
	CEventSystem::Delete(true);
	CContainer::Delete(true);
	CImGuiManager::Delete(true);

	DX11Uninit();
}
