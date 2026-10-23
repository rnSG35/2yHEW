#pragma once
#include <d3d11.h>
#include <wrl/client.h>

class Texture;

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
	void Draw(Texture* texture,
		float x,
		float y,
		float width,
		float height);

	//終了処理
	void UnInitialize();
private:
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

	//サンプラーステート作成
	bool CreateSamplerState();

	//ブレンドステート作成
	bool CreateBlendState();

private:
	//頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//頂点の並び方をDirectXに伝える
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//四角形の頂点を保存する
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	//テクスチャーの色を取得する方法
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	//png画像の透過を有効にする
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

	//画面サイズ
	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;


};

