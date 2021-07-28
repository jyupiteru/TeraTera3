/**
 * @file 	game.cpp
 * @author jupiter
 * @brief 	ゲームの本体ループ処理等記述しているcpp
 */
#include "game.h"

#include "Timer/CTimer.h"
#include "Managers/ImGuiSystem/ImGuiHeaders.h"
#include "EventSystem/CEventSystem.h"
#include "DebugLog/CDebugLog.h"
#include "ComSystem/ComSystem.h"
#include "ComSystem/Core/ObjectGenerator.h"
#include "Managers/SceneSystem/CSceneManager/CSceneManager.h"
#include "Managers/ResourceContainer/CContainer.h"
#include "Managers/TextureManager/CTextureManager.h"
#include "Managers/ShaderManager/CShaderManager.h"
#include "Managers/ShadowManager/CShadowManager.h"

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
	CDebugLog::GetInstance().Draw("Use TeraTera Ver3 Framework");

	CContainer::Create();

	CTextureManager::Create();

	bool sts;

	// DX11初期処理
	CDirectXGraphics::Create();
	sts = CDirectXGraphics::GetInstance().Init(hwnd, width, height, fullscreen);
	if (!sts)
	{
		MessageBox(hwnd, TEXT("DX11 init error"), TEXT("error"), MB_OK);
		return false;
	}

	CDebugLog::GetInstance().Draw("Set DX11");

	//SetTransformの初期化
	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts)
	{
		MessageBox(NULL, TEXT("SetTransform error"), TEXT("Error"), MB_OK);
		return false;
	}

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);
	CDebugLog::GetInstance().Draw("Set DirectInput");

	CShaderManager::Create();
	CDebugLog::GetInstance().Draw("Set ShaderManager");

	CShadowManager::Create();
	CDebugLog::GetInstance().Draw("Set ShadowManager");

	//ImGuiを管理するかマネージャーの生成と初期化
	CImGuiManager::Create();
	CImGuiManager::GetInstance().Init(hwnd);
	CDebugLog::GetInstance().Draw("Set CImGuiManager");

	//ウインドウを１個生成
	unsigned int windowid = CImGuiManager::GetInstance().CreateImGuiWindow();
	auto windowdata = CImGuiManager::GetInstance().GetImGuiWindow(windowid);

	{
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
	}

	CEventSystem::Create();
	CDebugLog::GetInstance().Draw("Set EventSystem");

	ObjectGenerator::Create();
	CDebugLog::GetInstance().Draw("Set ObjectGenerator");

	{
		// カメラ変換行列初期化
		XMFLOAT3 eye = {0, 0, -30};	 // 視点
		XMFLOAT3 lookat = {0, 0, 0}; // 注視点
		XMFLOAT3 up = {0, 1, 0};	 // 上向きベクトル

		//カメラオブジェクトを生成
		auto camera = GameObject::MakeNewObject("Camera", E_TYPE_OBJECT::NONE);
		camera->AddComponent<ComCamera>()->SetCamera(eye, lookat, up);

		camera->GetComponent<ComCamera>()->SetProjection(1.0f, 100000.0f,
														 XM_PIDIV2, SCREEN_WIDTH, SCREEN_HEIGHT);
		camera->AddComponent<ComShader>();
		camera->DontDestroyOnLoad();
		camera->m_objectUpdatePriority.SetValue(-20);
	}
	{
		// 平行光源初期化
		DirectX::XMFLOAT3 lightdir = {0, 100, -50};
		//シーンに設置するライトを生成
		auto light = GameObject::MakeNewObject("Light", E_TYPE_OBJECT::NONE);

		ComLight *comlight = light->AddComponent<ComLight>();
		comlight->m_ambientColor.SetValue(60, 60, 60);
		comlight->m_directionalColor.SetValue(155, 155, 155);

		light->m_transform->m_worldPosition.SetValue(lightdir.x, lightdir.y, lightdir.z);

		light->m_objectUpdatePriority.SetValue(-20);
		light->DontDestroyOnLoad();
	}

	// アルファブレンド有効にする
	CDirectXGraphics::GetInstance().TurnOnAlphaBlending();

	CSceneManager::Create();
	CDebugLog::GetInstance().Draw("Set CSceneManager");

	{
		//デバッグの表示欄に登録する処理
		CImGuiManager::GetInstance().SetImGuiFunction("ObjectList", &ObjectGenerator::GetInstance(), "Menu");
		CImGuiManager::GetInstance().SetImGuiFunction("Objects", std::bind(&ObjectGenerator::ImGuiDrawObjects, &ObjectGenerator::GetInstance(), std::placeholders::_1), "Menu");
		CImGuiManager::GetInstance().SetImGuiFunction("DrawLayer", std::bind(&ObjectGenerator::ImGuiDrawDrawLayer, &ObjectGenerator::GetInstance(), std::placeholders::_1), "Menu");
		CImGuiManager::GetInstance().SetImGuiFunction("ShaderData", &CShaderManager::GetInstance(), "Menu");

		CImGuiManager::GetInstance().SetImGuiFunction("SceneList", &CSceneManager::GetInstance(), "Menu");
		CImGuiManager::GetInstance().SetImGuiFunction("EventSystem", &CEventSystem::GetInstance(), "Menu");
		CImGuiManager::GetInstance().SetImGuiFunction("Timer", &CTimer::GetInstance(), "Menu");
		CImGuiManager::GetInstance().SetImGuiFunction("ShadowManager", &CShadowManager::GetInstance(), "Menu");

		//ImGuiの初期の欄で表示する内容を登録
		windowdata->SetImGuiFunction("Menu", "Project Property", true);
		windowdata->SetImGuiFunction("Menu", "Window Details", true);
		windowdata->SetImGuiFunction("Menu", "ObjectList", true);
		windowdata->SetImGuiFunction("Menu", "Objects", true);
		windowdata->SetImGuiFunction("Menu", "SceneList", true);
		windowdata->SetImGuiFunction("Menu", "EventSystem", true);
	}

	//GameのInit後にやらないと読み込み処理の時間が反映されてしまうので
	CTimer::GetInstance().Update();

	CDebugLog::GetInstance().Draw("End GameInit");

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

	CDebugLog::GetInstance().Update();
}

//================================================================================================
//================================================================================================

void GameDraw()
{
	// ターゲットバッファクリア
	float clearcolor[4] = {0.0f, 0.0f, 1.0f, 1.0f}; //red,green,blue,alpha

	// レンダリング前処理
	CDirectXGraphics::GetInstance().BeforeDraw(clearcolor);

	CShadowManager::GetInstance().CreateShadowMap();

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
	CSceneManager::Delete(true);
	ObjectGenerator::Delete(true);
	CEventSystem::Delete(true);
	CContainer::Delete(true);
	CTextureManager::Delete(true);
	CImGuiManager::Delete(true);
	CShaderManager::Delete(true);
	CDirectXGraphics::Delete(true);
	CShadowManager::Delete(true);
}
