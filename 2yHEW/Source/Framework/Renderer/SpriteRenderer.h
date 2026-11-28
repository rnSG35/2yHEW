#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <numbers>
#include "../Math/MathTypes.h"

class Texture;

/// <summary>
/// スプライトの合成方法
/// </summary>
enum class eSpriteBlendMode
{
	ALPHA,	//通常の半透明
	ADDITIVE//光などに使う加算合成
};

class SpriteRenderer
{
public:
	/// <summary>
	/// シングルトン取得
	/// </summary>
	/// <returns></returns>
	static SpriteRenderer& GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="screenWidth">ウィンドウ横幅</param>
	/// <param name="screenHeight">ウィンドウ縦幅</param>
	/// <returns>true:成功　失敗:false</returns>
	bool Initialize(UINT screenWidth, UINT screenHeight);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pTexture">描画する画像</param>
	/// <param name="x">画像中心のX座標</param>
	/// <param name="y">画像中心のY座標</param>
	/// <param name="width">描画横幅</param>
	/// <param name="height">描画縦幅</param>
	/// <param name="color">スプライトにかける色</param>
	/// <param name="blendMode">合成方法</param>
	/// <param name="rotationDegrees">画像の中心を軸にした回転(度)、プラスで反時計回り</param>
	void Draw(Texture* pTexture,
		float x,
		float y,
		float width,
		float height,
		const Float4& color,
		eSpriteBlendMode blendMode = eSpriteBlendMode::ALPHA,
		float rotationDegrees = 0.0f);

	/// <summary>
	/// 終了処理
	/// </summary>
	void UnInitialize();

	/// <summary>
	/// カメラの変換行列を設定
	/// </summary>
	/// <param name="viewProjectionMatrix">Camera::GetViewProjectionMatrix()で作った行列</param>
	/// <remarks>シーンの描画の最初に1回呼ぶ。以降のDraw()はすべてこのカメラで描画される</remarks>
	void SetViewProjection(const DirectX::XMMATRIX& viewProjectionMatrix);

	/// <summary>
	/// 画面の横幅を取得
	/// </summary>
	/// <returns></returns>
	UINT GetScreenWidth() const { return m_screenWidth; }
	/// <summary>
	/// 画面の縦幅を取得
	/// </summary>
	/// <returns></returns>
	UINT GetScreenHeight() const { return m_screenHeight; }

private:
	/// <summary>
	/// HLSLのConstBufferと同じ並びにする
	/// </summary>
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

	/// <summary>
	/// シェーダーファイル読み込み
	/// </summary>
	/// <param name="csoPath">csoファイルのパス</param>
	/// <param name="hlslPath">hlslファイルのパス</param>
	/// <param name="shaderBlob">ComPtr<ID3DBlob>&</param>
	/// <returns></returns>
	bool LoadVertexShaderBlob(
		const wchar_t* csoPath,
		const wchar_t* hlslPath,
		Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob);
	/// <summary>
	/// シェーダーファイル読み込み
	/// </summary>
	/// <param name="csoPath">csoファイルのパス</param>
	/// <param name="hlslPath">hlslファイルのパス</param>
	/// <param name="shaderBlob">ComPtr<ID3DBlob>&</param>
	/// <returns></returns>
	bool LoadPixelShaderBlob(
		const wchar_t* csoPath,
		const wchar_t* hlslPath,
		Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob);

	/// <summary>
	/// csoからシェーダー作成
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool CreateShaders();
	/// <summary>
	/// 頂点バッファ作成
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool CreateVertexBuffer();
	/// <summary>
	/// 定数バッファ作成	
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool CreateConstantBuffer();
	/// <summary>
	/// サンプラーステート作成
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool CreateSamplerState();
	/// <summary>
	///ブレンドステート作成
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool CreateBlendState();
	/// <summary>
	/// 深度ステンシルステート作成
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	/// <remarks>2Dスプライトでは深度テストを使わず、書いた順番で前後関係を決めるため</remarks>
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

	//カメラの変換行列
	//SetViewProjection()で受け取り、Drawのたびに定数バッファに送る
	//XMMATRIXはメンバ変数に置くと並びの問題が起きやすいため、保存用のXMFLOAT4X4で持つ
	DirectX::XMFLOAT4X4 m_viewProjection = {};

	//画面サイズ
	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;
};