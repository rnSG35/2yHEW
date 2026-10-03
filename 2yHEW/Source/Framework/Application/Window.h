#pragma once

#include <Windows.h>

class Window
{
public:
	//ウィンドウ作成
	bool Create(int width, int height, const wchar_t* title);

	//ウィンドウハンドル取得
	HWND GetHwnd() const { return m_hWnd; }

private:
	//Windowsから送られてくるメッセージの処理
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	//ウィンドウの実体
	HWND m_hWnd = nullptr;
};