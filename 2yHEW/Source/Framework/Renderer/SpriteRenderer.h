#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

class Texture;

//スプライトの合成方法
enum class eSpriteBlendMode
{
	ALPHA,	//通常の半透明
	ADDITIVE//光などに使う加算合成
};

class SpriteRenderer
{
public:
	//シングルトン取得
	static SpriteRenderer& GetInstance();

	//初期化
	bool Initialize(UINT screenWidth, UINT screenHeight);

	//描画
	//texture：描画する画像
	//x		 ：画像左上のX座標
	//y		 ：画像左上のY座標
	//width	 ：描画横幅
	//height ：描画縦幅
	//color  :スプライトにかける色
	//blendMode:合成方法
	void Draw(Texture* pTexture,
		float x,
		float y,
		float width,
		float height,
		const DirectX::XMFLOAT4& color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		eSpriteBlendMode blendMode = eSpriteBlendMode::ALPHA);

	//終了処理
	void UnInitialize();
private:
	//HLSLのConstBufferと同じ並びにする
	struct ConstantBufferData
	{
		DirectX::XMFLOAT4 vertexColor;
		DirectX::XMFLOAT4X4 matrixTex;
		DirectX::XMFLOAT4X4 matrixProj;
		DirectX::XMFLOAT4X4 matrixWorld;
	};

	SpriteRenderer() = default;
	~SpriteRenderer() = default;

	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	//シェーダーファイル読み込み
	bool LoadVertexShaderBlob(
		const wchar_t* csoPath,
		const wchar_t* hlslPath,
		Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob);
	bool LoadPixelShaderBlob(
		const wchar_t* csoPath,
		const wchar_t* hlslPath,
		Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob);

	//csoからシェーダー作成
	bool CreateShaders();
	//頂点バッファ作成
	bool CreateVertexBuffer();
	//定数バッファ作成
	bool CreateConstantBuffer();
	//サンプラーステート作成
	bool CreateSamplerState();
	//ブレンドステート作成
	bool CreateBlendState();
	//深度ステンシルステート作成
	//2Dスプライトでは深度てえストを使わず、書いた順番で前後関係を決めるため
	bool CreateDepthStencilState();

private:
	//頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//頂点の並び方をDirectXに伝える
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//四角形の頂点を保存する
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	//
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	//テクスチャーの色を取得する方法
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	//png画像の透過を有効にする
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaBlendState;
	//レーザーの光などに使う加算合成
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_additiveBlendState;
	//新でテストと深度書き込みを無効にする設定
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	
	//画面サイズ
	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;
};