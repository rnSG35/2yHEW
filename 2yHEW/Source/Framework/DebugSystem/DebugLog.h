#pragma once

#ifdef _DEBUG

class DebugLog
{
public:

	//---------------------------------------------------------
	// ログ出力
	//---------------------------------------------------------
	static void Write(const char* text);

	//---------------------------------------------------------
	//ログを空にする
	//---------------------------------------------------------
	static void Clear();
};

// 使いやすいようにマクロ化
#define DebugLogWrite(text) DebugLog::Write(text)
#define DebugLogClear() DebugLog::Clear()
#else
//---------------------------------------------------------
//Releaseの際は何もしない
//---------------------------------------------------------
#define DebugLogWrite(text)
#define DebugLogClear()
#endif