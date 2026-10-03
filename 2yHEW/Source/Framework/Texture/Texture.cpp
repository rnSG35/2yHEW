#include "Texture.h"
#include "../Graphics/Graphics.h"
#include <wincodec.h>
#include <vector>
#pragma comment(lib,"windowscodecs.lib")
using Microsoft::WRL::ComPtr;

//---------------------------------------------------------
//テクスチャ読み込み
//---------------------------------------------------------
bool Texture::Load(const std::string& filePath)
{
	//---------------------------------------------------------
	//WICファクトリ生成、PNG JPG BMPなどの画像を読み込むために使用
	//---------------------------------------------------------
	ComPtr<IWICImagingFactory> factory;

	HRESULT hr =
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(factory.GetAddressOf()));

	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//文字列変換、WICはstringを使うためstring->wstring変換する
	//---------------------------------------------------------
	std::wstring wPath(filePath.begin(), filePath.end());

	//---------------------------------------------------------
	//画像ファイル読み込み
	//---------------------------------------------------------
	ComPtr<IWICBitmapDecoder> decoder;
	hr = factory->CreateDecoderFromFilename(
		wPath.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		decoder.GetAddressOf());

	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//画像の戦闘フレーム取得・pngやjpgは通所１フレーム
	//---------------------------------------------------------
	ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, frame.GetAddressOf());

	if (FAILED(hr)) { return false; }



	//---------------------------------------------------------
	//RGBAに変換、GPUに転送しやすくするため
	//---------------------------------------------------------
	ComPtr<IWICFormatConverter> converter;

	hr = factory->CreateFormatConverter(converter.GetAddressOf());

	if (FAILED(hr)) { return false; }

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);

	if (FAILED(hr)) { return false; }


	//---------------------------------------------------------
	//画像サイズ取得
	//---------------------------------------------------------
	UINT width = 0;
	UINT height = 0;
	hr = converter->GetSize(&width, &height);

	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//RGBAデータ取得
	//---------------------------------------------------------
	std::vector<unsigned char>pixels;
	pixels.resize(width * height * 4);

	hr = converter->CopyPixels(
		nullptr,
		width * 4,
		static_cast<UINT>(pixels.size()),
		pixels.data());

	if (FAILED(hr)) { return false; }

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

	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	//---------------------------------------------------------
	//画像をGPUに転送
	//---------------------------------------------------------
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = pixels.data();
	initData.SysMemPitch = width * 4;

	hr = Graphics::GetInstance().
		GetDevice()->
		CreateTexture2D(
			&texDesc,
			&initData,
			m_texture.GetAddressOf()
		);	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//SRV作成、PixelShaderから参照するために必要
	//---------------------------------------------------------
	hr = Graphics::GetInstance().
		GetDevice()->
		CreateShaderResourceView(
			m_texture.Get(),
			nullptr,
			m_srv.GetAddressOf()
		);
	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//全処理が成功したら成功を返す
	//---------------------------------------------------------
	return true;
}

//---------------------------------------------------------
//PixelShaderに設定
//---------------------------------------------------------
void Texture::SetPS(UINT slot)
{
	Graphics::GetInstance().
		GetContext()->
		PSSetShaderResources(
			slot,
			1,
			m_srv.GetAddressOf()
		);
}
