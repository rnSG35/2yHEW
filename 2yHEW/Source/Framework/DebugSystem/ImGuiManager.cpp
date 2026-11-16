#include "ImguiManager.h"

#ifdef _DEBUG

#include "../../../thirdParty/imgui/include/imgui.h"
#include "../../../thirdParty/imgui/include/imgui_impl_win32.h"
#include "../../../thirdParty/imgui/include/imgui_impl_dx11.h"
#include "DebugLog.h"

//Imguiのwin32用入力処理
//imgui_impl_win32.hでは、windows.hとの衝突を避けるために宣言がコメントアウトされている
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

namespace
{
	//初期化が終わっているか
	bool s_isInitialized = false;
}

//---------------------------------------------------------
//初期化
//---------------------------------------------------------

bool ImGuiManager::Initialize(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (hwnd == nullptr || pDevice == nullptr || pContext == nullptr)
	{
		DebugLogWrite("ImGui初期化の引数がnullptr / ImGui Initialize argument is nullptr");
		return false;
	}

	//バージョン確認
	IMGUI_CHECKVERSION();

	//ImGuiの作業領域を作る
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	//キーボードでImGuiの項目を移動できるようにする
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//日本語フォントを読み込む
	io.Fonts->AddFontFromFileTTF(
		"C:/Windows/Fonts/meiryo.ttc",
		18.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese());

	//見た目を暗めに
	ImGui::StyleColorsDark();

	//Win32(ウィンドウ・入力)との接続
	if (!ImGui_ImplWin32_Init(hwnd))
	{
		DebugLogWrite("ImGui_ImplWin32_Init失敗 / Failed to ImGui_ImplWin32_Init");
		return false;
	}

	//DirectX11(描画)との接続
	if (!ImGui_ImplDX11_Init(pDevice, pContext))
	{
		DebugLogWrite("ImGui_ImplDX11_Init失敗 / Failed to ImGui_ImplDX11_Init");
		return false;
	}

	s_isInitialized = true;
	DebugLogWrite("ImGui初期化成功 / ImGui Inizeialization Successful");
	return true;
}

//---------------------------------------------------------
//フレーム開始
//---------------------------------------------------------
void ImGuiManager::BeginFrame()
{
	if (!s_isInitialized)
	{
		return;
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//---------------------------------------------------------
//ImGuiの描画
//---------------------------------------------------------
void ImGuiManager::EndFrame()
{
	if (!s_isInitialized)
	{
		return;
	}
	//このフレームで作った画面を、描画用のデータにまとめる
	ImGui::Render();
	//まとめたデータをDirectX11で描画する
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

//---------------------------------------------------------
//終了処理
//---------------------------------------------------------
void ImGuiManager::UnInitialize()
{
	if (!s_isInitialized)
	{
		return;
	}
	//初期化と逆の順番で終了
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	s_isInitialized = false;
}

//---------------------------------------------------------
//入力をImGuiに渡す
//---------------------------------------------------------
bool ImGuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!s_isInitialized)
	{
		return false;
	}
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp) != 0;
}
#endif