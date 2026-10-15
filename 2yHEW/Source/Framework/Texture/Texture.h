#pragma once

#include <d3d11.h>
#include <wrl/client.h>
//#include <string>

//TextureLoaderから非公開メンバにアクセスできるようにする
class TextureLoader;

class Texture
{
public:
	//shared_ptrがTextureを破棄できるようにpublicにする
	~Texture() = default;

	//テクスチャ読み込み
	// filePath : 読み込む画像ファイルのパス(png jpg bmpなど)
	//bool Load(const std::string& filePath);

	//ピクセルシェーダーへテクスチャーを設定
	//slot : シェーダーのテクスチャスロット番号(t0, t1, t2...)
	void SetPS(UINT slot = 0) const;

	//SRV取得
	ID3D11ShaderResourceView* GetSRV() const;
	
	//画像サイズ取得
	UINT GetWidth() const;
	UINT GetHeight() const;

private:

	//Textureの作成はTextureLoaderだけが行う
	friend class TextureLoader;

	Texture() = default;

	//GPU上のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;

	//シェーダーリソースビュー(SRV)、シェーダーからテクスチャを参照するためのビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

	//画像サイズ
	UINT m_width = 0;
	UINT m_height = 0;
};