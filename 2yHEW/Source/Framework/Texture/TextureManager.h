#pragma once
#include <unordered_map>
#include <memory>
#include <string>

class Texture;

//テクスチャ管理クラス
class TextureManager
{
public:
	//シングルトン取得
	static TextureManager& GetInstance();

	//テクスチャ読み込み
	//引数：ファイルパス(読み込む画像ファイル)
	//読み込めていない場合はTextureLoaderで読み込む
	std::shared_ptr<Texture> LoadTexture(const std::string& filePath);

	//管理中のテクスチャーをすべて解放
	void ClearTexture();

private:
	//コンストラクタ・デストラクタ
	TextureManager() = default;
	~TextureManager() = default;

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

private:
	//読み込み済みテクスチャ一覧
	//Key : ファイルパス
	//Value : テクスチャ
	std::unordered_map<std::string, std::shared_ptr<Texture>>m_textures;
};