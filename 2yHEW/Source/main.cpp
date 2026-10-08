#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS

//Application.hをインクルード
#include "Framework/Application/Application.h"
#include <combaseapi.h>
#include "../Source/Framework/DebugSystem/DebugLog.h"
// NVIDIA Optimus 対応：dGPUを優先
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

// AMD PowerXpress 対応：dGPUを優先
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

static const int g_nWindowWidth = 1920;
static const int g_nWindowHeight = 1080;

//---------------------------------------------------------
//エントリーポイント
//---------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	DebugLogClear();
	//---------------------------------------------------------
	//COM初期化
	//TextureクラスでWICを使用している
	//WIC内部でCOMというWindows機能を利用するためアプリ起動時にCOMを初期化
	//---------------------------------------------------------
	HRESULT hr = CoInitializeEx(
		nullptr,
		COINIT_MULTITHREADED);

	if (FAILED(hr)) { return 0; }


	//アプリケーションのシングルトンインスタンスを取得
	Application& App = Application::GetInstance();

	//ウィンドウ作成
	if (!App.AppInit(nShowCmd, false, g_nWindowWidth, g_nWindowHeight))
	{
		CoUninitialize();
		return 0;
	}

	//メインループ
	App.AppLoop();

	//終了処理
	App.UnInit();

	//COM終了
	CoUninitialize();

	return 0;
}