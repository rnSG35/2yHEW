#include "DebugLog.h"
#ifdef _DEBUG
#include <fstream>
#include <string_view>

namespace
{
	// ログファイル
	const char* FILE_PATH = "DebugLog/log.txt";

	//何もしないままだとlog.txtに絶対パスで書き込まれるので、Sourceフォルダからの相対パスに変換
	std::string_view ToRelativePath(std::string_view fullPath)
	{
		//Windowsの区切り文字「\」で探す
		size_t sourcePosition = fullPath.find("\\Source\\");

		//見つからなければ「/」でも探す
		if (sourcePosition == std::string_view::npos)
		{
			sourcePosition = fullPath.find("/Source/");
		}

		//Sourceフォルダが見つからない場合は、そのまま返す
		if (sourcePosition == std::string_view::npos)
		{
			return fullPath;
		}

		//先頭の区切り文字を飛ばして「Source...」から返す
		return fullPath.substr(sourcePosition + 1);
	}

}

//---------------------------------------------------------
// ログ出力
//---------------------------------------------------------
void DebugLog::Write(const char* text, const std::source_location& location)
{
	// 追記モードで開く
	std::ofstream file(FILE_PATH, std::ios::app);

	if (!file)
	{
		return;
	}

	file << "[FilePath：" << ToRelativePath(location.file_name())
		<< "：Line" << location.line()
		<< "]" << text << std::endl;

	file.close();
}

//---------------------------------------------------------
//ログ削除
//---------------------------------------------------------
void DebugLog::Clear()
{
	std::ofstream file(FILE_PATH, std::ios::trunc);
}

#endif