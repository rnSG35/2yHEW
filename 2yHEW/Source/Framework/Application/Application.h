#pragma once
#include "Window.h"
class Application
{
public:

	//---------------------------------------------------------
	//シングルトン取得
	//---------------------------------------------------------
	static Application& GetInstance()
	{
		static Application instance;
		return instance;
	}

	bool AppInit(int nShowCmd,bool isFullScreen,int screenWidth,int screenHeight);
	void MainLoop();
	void UnInit();

	//---------------------------------------------------------
	//ウィンドウ取得
	//---------------------------------------------------------
	Window& GetWindow() { return m_window; }

	//---------------------------------------------------------
	//HWND取得
	//---------------------------------------------------------
	HWND GetHwnd() { return m_window.GetHwnd(); }

private:
	//コンストラクタ・デストラクタ
	Application() = default;
	~Application() = default;

	//シングルトンをコピーできると、Applicationが複数存在してしまう
	//それをコンパイル時に防ぐため、コピー処理を削除する
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(const Application&&) = delete;
	Application& operator=(const Application&&) = delete;

	Window m_window;
};