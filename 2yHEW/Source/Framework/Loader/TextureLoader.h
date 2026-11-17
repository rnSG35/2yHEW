#pragma once

#include <memory>
#include <string>

class Texture;

class TextureLoader
{
public:
	//画像ファイルからTextureを作成
	static std::shared_ptr<Texture> Load(const std::string& filePath);
private:
	TextureLoader() = delete;
};