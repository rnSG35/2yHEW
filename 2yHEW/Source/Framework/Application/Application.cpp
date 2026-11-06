#include "Application.h"
#include "../Graphics/Graphics.h"
#include "../../Game/Core/Game.h"
#include "../DebugSystem/DebugLog.h"

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

	return true;
}

//---------------------------------------------------------
//メインループ
//---------------------------------------------------------
void Application::AppLoop()
{
	MSG msg{};

	Game game;

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
			//ここにゲームの更新処理や描画処理を追加する
			//Update
			//Draw
			game.Update();

			Graphics::GetInstance().BeginFrame();

			game.Draw();

			Graphics::GetInstance().EndFrame();
			
			//初期化が成功し、最初のフレームも表示できたら起動時のログを消して、ゲーム中のログだけにする
			//表示に失敗した場合はクリアしない
			if (!isStartupLogCleared && Graphics::GetInstance().IsLastPresentSucceeded())
			{
				DebugLogClear();
				DebugLogWrite("起動処理完了 / Startup Complete");

				isStartupLogCleared = true;
				//
			}
		}
	}
	game.Exit();
}

//---------------------------------------------------------
//解放処理
//---------------------------------------------------------
void Application::UnInit()
{
	//ここでRendererやInput、Sound、Resourceの解放処理を行う
}