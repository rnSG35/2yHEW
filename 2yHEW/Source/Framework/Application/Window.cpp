#include "Window.h"
#include "Application.h"
#include "../DebugSystem/DebugLog.h"
//---------------------------------------------------------
//ウィンドウ生成
//---------------------------------------------------------
bool Window::Create(int width, int height, const wchar_t* title)
{
	//ウィンドウクラス情報
	WNDCLASSEX wc{};

	wc.cbSize = sizeof(WNDCLASSEX);				//構造体サイズを設定
	wc.lpfnWndProc = WndProc;					//メッセージ処理関数
	wc.hInstance = GetModuleHandle(nullptr);	//実行中のアプリケーション情報
	wc.lpszClassName = L"WindowClass";			//ウィンドウクラス名

	//Windowsにウィンドウクラスを登録
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}


	//ウィンドウにタイトルバーとか枠が含まれているので、サイズを正しくするための対策
	RECT windowRect =
	{
		0,
		0,
		width,
		height
	};
	AdjustWindowRect(
		&windowRect,
		WS_OVERLAPPEDWINDOW,
		FALSE
	);

	//ウィンドウを生成
	m_hWnd = CreateWindowEx(
		0,
		L"WindowClass",
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		/*width*/windowRect.right - windowRect.left,
		/*height*/windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr
	);

	//作成失敗したときはfalseを返す
	if (!m_hWnd)
	{
		return false;
	}

	//ウィンドウ表示
	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}

//---------------------------------------------------------
//Windowsメッセージ処理
//---------------------------------------------------------
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
	{	//ウィンドウが閉じられたらWM_QUITを送ってゲームループ終了
		PostQuitMessage(0);

	}
	break;
	case WM_CLOSE: //「x」ボタンが押されたら
	{
		DestroyWindow(hWnd); // メッセージループにWM_QUITを送る
	}
	break;
	case WM_KEYDOWN:
	{	//ESCキーが押されたらウィンドウを閉じる
		if (wp == VK_ESCAPE)
		{
			DestroyWindow(hWnd);

		}
		return 0;

	}
	break;
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(nullptr, IDC_ARROW)); // 矢印カーソルを設定
	}
	break;

	default:
		//未処理メッセージはWindows標準処理に任せる
		return DefWindowProc(hWnd, msg, wp, lp);
	}

	return 0;
}