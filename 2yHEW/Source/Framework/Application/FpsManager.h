#pragma once

#include <Windows.h>

class FpsManager
{
public:
	//フレームレート制限
	static constexpr int FPS_LIMIT = 60;

	FpsManager();
	~FpsManager() = default;

	//次のフレームを処理してもいいか判定
	//1/60秒経過していればtrue
	bool IsReadyToUpdate();

	//計測したフレームレートを取得
	float GetFps() const { return m_fps; }

	//前のフレームからの経過時間を取得
	float GetDeltaTime() const { return m_deltaTime; }

	//フレームレートの表示を更新するタイミングか
	bool IsFpsUpdated() const { return m_isFpsUpdated; }


private:
	//フレームレートを計算する感覚
	static constexpr double FPS_MEASURE_INTERVAL = 0.5;

	//タイマーが1秒間に進む回数
	long long m_frequency = 0;



	//前回フレームを処理したときのタイマーの値
	long long m_lastFrameCount = 0;

	//フレームレートの計測を始めたときのタイマーの値
	long long m_measureStartCnt = 0;

	//計測中に処理したフレーム数
	int m_frameCnt = 0;

	//計測したフレームレート
	float m_fps = 0.0f;

	//前のフレームからの経過時間
	float m_deltaTime = 0.0f;

	//フレームレートが更新された直後かどうか
	bool m_isFpsUpdated = false;
};