/**
 * @file 	game.cpp
 * @author jupiter
 * @brief 	ゲームの本体ループ処理等記述しているcpp
 * @version 1.0
 * @date 	2020-07-11
 */
#include "game.h"

#include "Timer/CTimer.h"
#include "ImGuiSystem/ImGuiHeaders.h"
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

	CDebugLog::GetInstance().Draw("DebugLog is Start");
	CDebugLog::GetInstance().Draw("Use TeraTera ver3 Framewark");

	CContainer::Create();

	bool sts;

	// DX11初期処理
	CDirectXGraphics::Create();
	sts = CDirectXGraphics::GetInstance().Init(hwnd, width, height, fullscreen);
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

	//ImGuiを管理するかマネージャーの生成と初期化
	CImGuiManager::Create();
	CImGuiManager::GetInstance().Init(hwnd);

	//ウインドウを１個生成
	unsigned int windowid = CImGuiManager::GetInstance().CreateImGuiWindow();
	auto windowdata = CImGuiManager::GetInstance().GetImGuiWindow(windowid);

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
		camera->AddComponent<ComShader>();
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

	// アルファブレンド有効にする
	CDirectXGraphics::GetInstance().TurnOnAlphaBlending();

	CSceneManager::Create();

	CImGuiManager::GetInstance().SetImGuiFunction("SceneList", &CSceneManager::GetInstance(), "Menu");
	CImGuiManager::GetInstance().SetImGuiFunction("EventSystem", &CEventSystem::GetInstance(), "Menu");

	windowdata->SetImGuiFunction("Menu", "Project Property", true);
	windowdata->SetImGuiFunction("Menu", "Window Details", true);
	windowdata->SetImGuiFunction("Menu", "ObjectList", true);
	windowdata->SetImGuiFunction("Menu", "Objects", true);
	windowdata->SetImGuiFunction("Menu", "SceneList", true);
	windowdata->SetImGuiFunction("Menu", "EventSystem", true);
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
	CEventSystem::GetInstance().Update();

	//シーンに存在しているオブジェクトのUpdateをぶん回し
	CSceneManager::GetInstance().Update();
}

//================================================================================================
//================================================================================================

void GameDraw()
{
	// ターゲットバッファクリア
	float clearcolor[4] = {0.0f, 0.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// レンダリング前処理
	CDirectXGraphics::GetInstance().BeforeDraw(clearcolor);

	//シーンに存在しているオブジェクトのDrawをぶん回し
	CSceneManager::GetInstance().Draw();

	CImGuiManager::GetInstance().Draw();

	// レンダリング後処理
	CDirectXGraphics::GetInstance().AfterDraw();
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
	CDirectXGraphics::Delete(true);
}
