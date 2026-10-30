#include "TextureLoader.h"

#include "DebugLog.h"
#include "../Graphics/Graphics.h"
#include "../Texture/Texture.h"
#include "../../../thirdParty/stb_image/stb_image.h"

//---------------------------------------------------------
//テクスチャ読み込み
//---------------------------------------------------------
std::shared_ptr<Texture> TextureLoader::Load(const std::string& filePath)
{
	int width = 0;
	int height = 0;
	int channelsCount = 0;

	//---------------------------------------------------------
	//stb_imageで画像読み込み
	//---------------------------------------------------------
	unsigned char* imageData;
	imageData = stbi_load(filePath.c_str(), &width, &height, &channelsCount, STBI_rgb_alpha);

	DebugLogWrite(filePath.c_str());
	if (imageData == nullptr)
	{
		DebugLogWrite("画像読込失敗 / Failed to load image");
		return nullptr;
	}
	//m_width = width;
	//m_height = height;

	//---------------------------------------------------------
	//読み込んだ画像の設定
	//---------------------------------------------------------
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = static_cast<UINT>(width);
	textureDesc.Height = static_cast<UINT>(height);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//---------------------------------------------------------
	//画像をCPUに転送
	//---------------------------------------------------------
	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = imageData;
	textureData.SysMemPitch = static_cast<UINT>(width * 4);

	//---------------------------------------------------------
	//Textureを生成
	//---------------------------------------------------------
	std::shared_ptr<Texture> texture(new Texture());

	//---------------------------------------------------------
	//GPU上にTexture2Dを作成
	//---------------------------------------------------------
	HRESULT createTexture2DResult = Graphics::GetInstance().GetDevice()->CreateTexture2D(
		&textureDesc,
		&textureData,
		texture->m_texture.GetAddressOf());


	//---------------------------------------------------------
	//CPU側の画像データはTexture2D作成後に解放
	//---------------------------------------------------------
	stbi_image_free(imageData);
	if (FAILED(createTexture2DResult))
	{
		DebugLogWrite("Texture2D作成失敗 / Failed to create Texture2D");
		return nullptr;
	}

	//---------------------------------------------------------
	//SshaderResourceView作成、PixelShaderから参照するために必要
	//---------------------------------------------------------
	HRESULT createSrvResult = Graphics::GetInstance().GetDevice()->CreateShaderResourceView(
		texture->m_texture.Get(),
		nullptr,
		texture->m_srv.GetAddressOf());

	if (FAILED(createSrvResult))
	{
		DebugLogWrite("ShaderResourceView作成失敗 / Failed to create ShaderResourceView");
		return nullptr;
	}

	//---------------------------------------------------------
	//画像サイズを保存
	//---------------------------------------------------------
	texture->m_width = static_cast<UINT>(width);
	texture->m_height = static_cast<UINT>(height);
	DebugLogWrite("Texture読み込み成功");
	return texture;
}
