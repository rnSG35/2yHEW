#include "Texture.h"
#include "../Graphics/Graphics.h"
#include "../../../thirdParty/stb_image/stb_image.h"
using Microsoft::WRL::ComPtr;

//---------------------------------------------------------
//テクスチャ読み込み
//---------------------------------------------------------
bool Texture::Load(const std::string& filePath)
{
	int width = 0;
	int height = 0;
	int channels = 0;

	//---------------------------------------------------------
	//stb_imageで画像読み込み
	//---------------------------------------------------------
	unsigned char* imageData;
	imageData = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (!imageData)
	{
		OutputDebugStringA("画像読込失敗\n");
		return false;
	}
	m_width = width;
	m_height = height;

	//---------------------------------------------------------
	//GPUテクスチャ生成
	//---------------------------------------------------------
	D3D11_TEXTURE2D_DESC texDesc = {};

	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//---------------------------------------------------------
	//画像をCPUに転送
	//---------------------------------------------------------
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = imageData;
	initData.SysMemPitch = width * 4;

	HRESULT hr = Graphics::GetInstance().GetDevice()->CreateTexture2D(
		&texDesc,
		&initData,
		m_texture.GetAddressOf());

	if (FAILED(hr)) { stbi_image_free(imageData); return false; }

	//---------------------------------------------------------
	//SRV作成、PixelShaderから参照するために必要
	//---------------------------------------------------------
	hr = Graphics::GetInstance().GetDevice()->CreateShaderResourceView(
		m_texture.Get(),
		nullptr,
		m_srv.GetAddressOf());

	if (FAILED(hr)) { stbi_image_free(imageData); return false; }

	//---------------------------------------------------------
	//全処理が成功したら成功を返す
	//---------------------------------------------------------
	stbi_image_free(imageData);
	return true;
}

//---------------------------------------------------------
//PixelShaderに設定
//---------------------------------------------------------
void Texture::SetPS(UINT slot)
{
	Graphics::GetInstance().GetContext()->PSSetShaderResources(
		slot,
		1,
		m_srv.GetAddressOf());
}
