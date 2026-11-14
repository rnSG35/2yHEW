#include "FpsManager.h"

//---------------------------------------------------------
//コンストラクタ
//---------------------------------------------------------
FpsManager::FpsManager()
{
	LARGE_INTEGER counter = {};

	//タイマーが１秒間に進む回数を取得
	QueryPerformanceFrequency(&counter);
	m_frequency = counter.QuadPart;

	//現在のタイマーの値を計測の開始点にする
	QueryPerformanceCounter(&counter);
	m_lastFrameCnt = counter.QuadPart;
	m_measureStartCnt = counter.QuadPart;
}

//---------------------------------------------------------
//次のフレームを処理してよいか判定
//---------------------------------------------------------
bool FpsManager::IsReadyToUpdate()
{
	//表示の更新は計測直後の1回だけにするため、毎回falseに戻す
	m_isFpsUpdated = false;

	LARGE_INTEGER counter = {};
	QueryPerformanceCounter(&counter);
	long long nowCnt = counter.QuadPart;

	//1/60秒分タイマーが進んでいなければ、処理しない
	//m_frequency / TARGET_FPSが「1/60秒」に当たるタイマーの量
	if (nowCnt < m_lastFrameCnt + m_frequency / FPS_LIMIT)
	{
		return false;
	}

	//前のフレームからの経過時間を秒で求める
	m_deltaTime = static_cast<float>(static_cast<double>(nowCnt - m_lastFrameCnt) / m_frequency);

	m_lastFrameCnt = nowCnt;

	//フレームレートを計測
	//0.5秒ごとに「処理したフレーム数 / 経過時間」で平均を出す
	m_frameCnt++;

	double measureSeconds = static_cast<double>(nowCnt - m_measureStartCnt) / m_frequency;

	if (measureSeconds >= FPS_MEASURE_INTERVAL)
	{
		m_fps = static_cast<float>(m_frameCnt / measureSeconds);

		//次の計測の為にリセット
		m_frameCnt = 0;
		m_measureStartCnt = nowCnt;
		m_isFpsUpdated = true;
	}
	return true;
}
