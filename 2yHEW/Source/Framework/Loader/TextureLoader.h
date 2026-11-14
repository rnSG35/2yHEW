#pragma once

#include <memory>
#include <string>

class Texture;

class TextureLoader
{
public:
	//‰æ‘œƒtƒ@ƒCƒ‹‚©‚çTexture‚ğì¬
	static std::shared_ptr<Texture> Load(const std::string& filePath);

private:
	TextureLoader() = delete;
};