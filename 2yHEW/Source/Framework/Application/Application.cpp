#include "Application.h"
#include "../Graphics/Graphics.h"
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

	//---------------------------------------------------------
	// DirectX初期化
	//
	// Window生成後でないとHWNDが存在しないため、
	// Graphicsの初期化はここで行う。
	//---------------------------------------------------------
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
void Application::MainLoop()
{
	MSG msg{};

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
			Graphics::GetInstance().BeginFrame();

			Graphics::GetInstance().EndFrame();
		}
	}
}


void Application::UnInit()
{
	//ここでRendererやInput、Sound、Resourceの解放処理を行う
}