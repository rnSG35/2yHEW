#include "Application.h"
#include "FpsManager.h"
#include "../Graphics/Graphics.h"
#include "../../Game/Core/GameManager.h"

#include "../DebugSystem/DebugLog.h"
#include "../DebugSystem/ImGuiManager.h"

//---------------------------------------------------------
//アプリケーション初期化
// Window生成
// Graphics初期化
// Input初期化
//---------------------------------------------------------
bool Application::AppInit(int nShowCmd, bool isFullScreen, int screenWidth, int screenHeight)
{
	//ウィンドウ生成
	if (!m_window.Create(screenWidth, screenHeight, L"HEW2026"))
	{
		return false;
	}

	// DirectX初期化
	// Window生成後でないとHWNDが存在しないため、
	// Graphicsの初期化はここで行う。
	if (!Graphics::GetInstance().Initialize(
		m_window.GetHwnd(),
		screenWidth,
		screenHeight))
	{
		return false;
	}

	//ImGui初期化
	if (!ImGuiManager::Initialize(
		m_window.GetHwnd(),
		Graphics::GetInstance().GetDevice(),
		Graphics::GetInstance().GetContext()))
	{
		return true;
	}

	return true;
}

//---------------------------------------------------------
//メインループ
//---------------------------------------------------------
void Application::AppLoop()
{
	MSG msg{};

	GameManager game;

	FpsManager fpsManager;

	if (!game.Init())
	{
		DebugLogWrite("Game初期化失敗 / Failed to Initialize Game class");
		return;
	}

	//起動時のログを消したかどうかを判定する変数
	//最初のフレームのみ判定するので、覚えておく
	bool isStartupLogCleared = false;

	//WM_QUITが送られるまでループ
	while (msg.message != WM_QUIT)
	{
		//Windowsからメッセージが来ているか確認
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//キーボード入力などを処理
			TranslateMessage(&msg);

			//WndProc関数にメッセージを送る
			DispatchMessage(&msg);
		}
		else
		{
			//1/60秒立っていなければ、更新も描画もしない
			//(１秒間の処理回数を60回に固定)
			if (!fpsManager.IsReadyToUpdate())
			{
				continue;
			}

			//シーンのUpdate()の中でImGuiの画面を作れるようにするためにgame.update()より前に
			ImGuiManager::BeginFrame();

			game.Update();

			//シーンからゲーム終了が依頼されたらループを抜ける
			if (game.IsQuitRequested())
			{
				DebugLogWrite("ゲーム終了 / Game Quit");
				
				//ウィンドウを閉じる
				DestroyWindow(m_window.GetHwnd());
				break;
			}

			Graphics::GetInstance().BeginFrame();

			game.Draw();

			//ゲーム描画の後に呼んで、ゲームより手前に描画
			ImGuiManager::EndFrame();

			Graphics::GetInstance().EndFrame();

			//キャプションにフレームレートを表示
			if (fpsManager.IsFpsUpdated())
			{
				wchar_t windowTitle[64];
				swprintf_s(windowTitle, L"2yHEW Scene = %ls FPS = %.2f", game.GetCurrentSceneName(), fpsManager.GetFps());

				SetWindowTextW(m_window.GetHwnd(), windowTitle);
			}

			//初期化が成功し、最初のフレームも表示できたら起動時のログを消して、ゲーム中のログだけにする
			//表示に失敗した場合はクリアしない
			if (!isStartupLogCleared && Graphics::GetInstance().IsLastPresentSucceeded())
			{
				DebugLogClear();
				DebugLogWrite("起動処理完了 / Startup Complete");

				isStartupLogCleared = true;
			}
		}
	}
	game.Exit();

	ImGuiManager::UnInitalize();
}

//---------------------------------------------------------
//解放処理
//---------------------------------------------------------
void Application::UnInit()
{
	//ここでRendererやInput、Sound、Resourceの解放処理を行う
}