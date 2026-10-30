#include "TextureManager.h"
#include "Texture.h"
#include "../Loader/TextureLoader.h"
#include "DebugLog.h"
//---------------------------------------------------------
//TextureManagerのシングルトンを取得
//---------------------------------------------------------
TextureManager& TextureManager::GetInstance()
{
	//関数内staticを使用すると、最初に呼ばれた時だけ生成
	//明示的なnew deleteが不要、自動で破棄される
	//管理クラスのシングルトンとして扱いやすい
	static TextureManager instance;

	return instance;
}

//---------------------------------------------------------
//テクスチャ読み込み
//---------------------------------------------------------
std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& filePath)
{
	//同じファイルが読み込まれているか確認、読み込み済みならそのTextureを返す
	//同じ画像を何度もGPUに作成すると、時間とメモリを無駄に消費するため
	//読み込み済みデータを再利用
	auto textureIterator = m_textures.find(filePath);

	if (textureIterator != m_textures.end())
	{
		////weak_ptrからshared_ptrを取得
		////元のTextureがまだ存在していれば、そのTextureを返す
		//std::shared_ptr<Texture> cachedTexture =
		//	textureIterator->second.lock();
		//if (cachedTexture)
		//{
		//	return cachedTexture;
		//}
		////weak_ptrだけが残っていて、Texture本体がすでに破棄されている場合は
		////古い管理情報を削除して、改めて読み込める状態にする
		//m_textures.erase(textureIterator);

		return textureIterator->second;
	}

	//新しいTextureを生成
	//make_sharedを使うことで、Texture本体と参照管理情報を効率よく確保できる
	std::shared_ptr<Texture> texture = TextureLoader::Load(filePath);

	//画像ファイルの読み込みに失敗した場合はnullptrを返す
	//失敗したTextureを管理表に登録しないことで、次回の読み込みを妨げないようにする
	if (texture == nullptr)
	{
		DebugLogWrite("TextureManagerの画像取得失敗 / TextureManager Failed to Retrieve Image");
		return nullptr;
	}

	//読み込み済みTextureをshared_ptrで保存する
	//同じ画像が要求された場合に再利用できるようにする
	m_textures.emplace(filePath, texture);

	return texture;
}

//---------------------------------------------------------
//管理中のテクスチャーをすべて解放
//---------------------------------------------------------
void TextureManager::ClearTexture()
{
	m_textures.clear();

	DebugLogWrite("TextureManager解放完了");
}