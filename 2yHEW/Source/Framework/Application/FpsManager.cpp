#include "FpsManager.h"

FpsManager::FpsManager()
{
	LARGE_INTEGER counter = {};

	//タイマーが１秒間に進む回数を取得
	QueryPerformanceFrequency(&counter);
	m_frequency = counter.QuadPart;

	//現在のタイマーの値を計測の開始点にする
	QueryPerformanceCounter(&counter);
	m_lastFrameCount = counter.QuadPart;
}

bool FpsManager::IsReadyToUpdate()
{
	return false;
}
