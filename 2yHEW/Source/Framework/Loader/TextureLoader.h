#pragma once

#include <memory>
#include <string>

class Texture;

class TextureLoader
{
public:
	/// <summary>
	/// 画像ファイルからTextureを作成
	/// </summary>
	/// <param name="filePath">画像ファイルのパス</param>
	/// <returns></returns>
	static std::shared_ptr<Texture> Load(const std::string& filePath);
private:
	TextureLoader() = delete;
};