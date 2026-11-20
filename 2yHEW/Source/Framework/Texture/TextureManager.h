#pragma once

#include <unordered_map>
#include <memory>
#include <string>

class Texture;

//テクスチャ管理クラス
class TextureManager
{
public:
	/// <summary>
	/// シングルトン取得
	/// </summary>
	/// <returns></returns>
	static TextureManager& GetInstance();

	/// <summary>
	/// テクスチャ読み込み、読み込めていない場合はTextureLoaderで読み込む
	/// </summary>
	/// <param name="filePath">ファイルパス(読み込む画像ファイル)</param>
	/// <returns></returns>
	std::shared_ptr<Texture> LoadTexture(const std::string& filePath);

	/// <summary>
	/// 管理中のテクスチャーをすべて解放
	/// </summary>
	void ClearTexture();

private:
	//コンストラクタ・デストラクタ
	TextureManager() = default;
	~TextureManager() = default;

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

private:
	/// <summary>
	/// Key : ファイルパス
	/// Value : テクスチャ
	/// </summary>
	std::unordered_map<std::string, std::shared_ptr<Texture>>m_textures;
};