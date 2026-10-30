#include "Texture.h"
#include "Graphics.h"
#include "stb_image.h"
#include "DebugLog.h"
using Microsoft::WRL::ComPtr;

//---------------------------------------------------------
//テクスチャ読み込み
//---------------------------------------------------------
//bool Texture::Load(const std::string& filePath)
//{
//	int width = 0;
//	int height = 0;
//	int channels = 0;
//
//	//---------------------------------------------------------
//	//stb_imageで画像読み込み
//	//---------------------------------------------------------
//	unsigned char* textureData;
//	textureData = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
//
//	if (!textureData)
//	{
//		DebugLogWrite("画像読込失敗 / Failed to load image");
//		return false;
//	}
//	m_width = width;
//	m_height = height;
//
//	//---------------------------------------------------------
//	//GPUテクスチャ生成
//	//---------------------------------------------------------
//	D3D11_TEXTURE2D_DESC texDesc = {};
//
//	texDesc.Width = width;
//	texDesc.Height = height;
//	texDesc.MipLevels = 1;
//	texDesc.ArraySize = 1;
//	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	texDesc.SampleDesc.Count = 1;
//	texDesc.Usage = D3D11_USAGE_DEFAULT;
//	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//
//	//---------------------------------------------------------
//	//画像をCPUに転送
//	//---------------------------------------------------------
//	D3D11_SUBRESOURCE_DATA initData = {};
//	initData.pSysMem = textureData;
//	initData.SysMemPitch = width * 4;
//
//	HRESULT createTexture2DResult = Graphics::GetInstance().GetDevice()->CreateTexture2D(
//		&texDesc,
//		&initData,
//		m_texture.GetAddressOf());
//
//	if (FAILED(createTexture2DResult)) { stbi_image_free(textureData); return false; }
//
//	//---------------------------------------------------------
//	//SRV作成、PixelShaderから参照するために必要
//	//---------------------------------------------------------
//	HRESULT createSrvResult = Graphics::GetInstance().GetDevice()->CreateShaderResourceView(
//		m_texture.Get(),
//		nullptr,
//		m_srv.GetAddressOf());
//
//	if (FAILED(createSrvResult)) { stbi_image_free(textureData); return false; }
//
//	//---------------------------------------------------------
//	//全処理が成功したら成功を返す
//	//---------------------------------------------------------
//	stbi_image_free(textureData);
//	return true;
//}

//---------------------------------------------------------
//PixelShaderに設定
// 一度ローカル変数へ入れているのは、
// PSSetShaderResources()がポインタ配列を受け取るため
//---------------------------------------------------------
void Texture::SetPS(UINT slot) const
{
	ID3D11ShaderResourceView* srv = m_srv.Get();

	Graphics::GetInstance().GetContext()->PSSetShaderResources(
		slot,
		1,
		&srv);
}
//---------------------------------------------------------
//SRV取得
//---------------------------------------------------------
ID3D11ShaderResourceView* Texture::GetSRV() const
{
	return m_srv.Get();
}

//---------------------------------------------------------
//画像サイズ取得
//---------------------------------------------------------
UINT Texture::GetWidth() const
{
	return m_width;
}
UINT Texture::GetHeight() const
{
	return m_height;
}