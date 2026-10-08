#pragma once

class DebugLog
{
public:

	//---------------------------------------------------------
	// ログ出力
	//---------------------------------------------------------
	static void Write(const char* text);
};

// 使いやすいようにマクロ化
#define DebugLogWrite(text) DebugLog::Write(text)