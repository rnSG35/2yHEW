#pragma once

#include <Windows.h>
#include <d3d11.h>

class ImGuiManager
{
public:
#ifdef _DEBUG
	//初期化
	static bool Initialize(
		HWND hwnd,
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	//フレーム開始
	//この後からImgui::Begin()などが使える
	static void BeginFrame();
	//Imguiの描画
	//ゲームより手前に表示するためにゲームの描画の後、Presentの前に呼ぶ
	static void EndFrame();

	//終了
	static void UnInitalize();

	//入力後Imguiに渡す
	//Imguiが入力を使った場合はtrueを返す
	static bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

#else
	//Release用
	static bool Initialize(HWND, ID3D11Device*, ID3D11DeviceContext*) { return true; }
	static void BeginFrame() {}
	static void EndFrame() {}
	static void UnInitialize() {}
	static bool WndProcHandler(HWND, UINT, WPARAM, LPARAM) { return false; }
#endif
};