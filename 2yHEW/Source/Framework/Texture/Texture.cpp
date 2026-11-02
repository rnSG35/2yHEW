#include "Texture.h"
#include "../Graphics/Graphics.h"
#include "../../../thirdParty/stb_image/stb_image.h"
#include "../DebugSystem/DebugLog.h"
using Microsoft::WRL::ComPtr;

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