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

	/// <summary>
	/// ピクセルシェーダーへテクスチャーを設定
	/// </summary>
	/// <param name="slot">シェーダーのテクスチャスロット番号(t0, t1, t2...)</param>
	void SetPS(UINT slot = 0) const;

	/// <summary>
	/// SRV取得
	/// </summary>
	/// <returns>ID3D11ShaderResourceView*</returns>
	ID3D11ShaderResourceView* GetSRV() const;

	/// <summary>
	/// 画像サイズ横幅取得
	/// </summary>
	/// <returns></returns>
	UINT GetWidth() const;
	/// <summary>
	/// 画像サイズ縦幅取得
	/// </summary>
	/// <returns></returns>
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