#include"../../Framework/Pch.h"
#include "SpriteRenderer.h"
#include "../Graphics/Graphics.h"
#include "../Texture/Texture.h"
#include "../DebugSystem/DebugLog.h"

//#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;

//スプライト用頂点
namespace
{
	struct sSpriteVertex
	{
		//画面上の頂点位置
		float position[4];

		float color[4];

		//画像のどこを使用するかを表す座標
		float uv[2];
	};
}

//---------------------------------------------------------
//シングルトン取得
//---------------------------------------------------------
SpriteRenderer& SpriteRenderer::GetInstance()
{
	static SpriteRenderer instance;
	return instance;
}

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
bool SpriteRenderer::Initialize(UINT screenWidth, UINT screenHeight)
{
	//描画位置を画面座標から変換するときに使用するため、画面サイズを保存しておく
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	if (m_screenWidth == 0 || m_screenHeight == 0)
	{
		DebugLogWrite("SpriteRendererの画面サイズが0 / The SpriteRenderer's screen size is 0");
		return false;
	}

	//シェーダー作成
	if (!CreateShaders())
	{
		DebugLogWrite("SpriteRendererのシェーダー作成失敗 / Failed to create a shader for SpriteRenderer");
		return false;
	}

	//頂点バッファ作成
	if (!CreateVertexBuffer())
	{
		DebugLogWrite("SpriteRendererの頂点バッファ作成失敗 /Failed to create a vertex buffer for SpriteRenderer");
		return false;
	}

	//定数バッファ作成
	if (!CreateConstantBuffer())
	{
		DebugLogWrite("SpriteRendererの定数バッファ作成失敗 / Failed to create ConstantBuffer for SpriteRenderer");
		return false;
	}

	//テクスチャの取得方法を作成
	if (!CreateSamplerState())
	{
		DebugLogWrite("SpriteRendererのSamplerState作成失敗 / Failed to create a SamplerState for SpriteRenderer");
		return false;
	}

	//png画像の透過を有効にする設定を作成
	if (!CreateBlendState())
	{
		DebugLogWrite("SpriteRendererのBlendState作成失敗 / Failed to create a BlendState for SpriteRenderer");
		return false;
	}

	//深度テストを使わない設定を作成
	if (!CreateDepthStencilState())
	{
		DebugLogWrite("SpriteRendererのDepthStencilState作成失敗 / Failed to create a DepthStencilState for SpriteRenderer");
		return false;
	}

	DebugLogWrite("SpriteRenderer初期化成功 / SpriteRenderer initialization successful");
	return true;
}

bool SpriteRenderer::LoadVertexShaderBlob(const wchar_t* csoPath, const wchar_t* hlslPath, ComPtr<ID3DBlob>& shaderBlob)
{
	//CSOがあるか確認
	HRESULT readShaderResult = D3DReadFileToBlob(
		csoPath, shaderBlob.GetAddressOf());
	if (SUCCEEDED(readShaderResult))
	{
		DebugLogWrite("VertexShader.cso読み込み成功 / VertexShader.cso loaded successfully");
		return true;
	}
	DebugLogWrite("VertexShader.csoが無いためHLSLをコンパイル / Compile HLSL because VertexShader.cso is missing");
	//HLSLをコンパイル
	ComPtr<ID3DBlob> errorBlob;
	HRESULT compileShaderResult = D3DCompileFromFile(
		hlslPath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
#ifdef _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
#endif
		0,
		shaderBlob.GetAddressOf(),
		errorBlob.GetAddressOf());

	if (FAILED(compileShaderResult))
	{
		if (errorBlob)
		{
			//DebugLogWrite(static_cast<const char*>(errorBlob->GetBufferPointer()));
		}
		DebugLogWrite("VertexShaderコンパイル失敗 / Vertex Shader Compilation Failed");
		return false;
	}
	DebugLogWrite("VertexShaderコンパイル成功 / Vertex Shader Compilation Successful");
	return true;
}

bool SpriteRenderer::LoadPixelShaderBlob(const wchar_t* csoPath, const wchar_t* hlslPath, ComPtr<ID3DBlob>& shaderBlob)
{
	//csoファイルが存在するか確認
	HRESULT readShaderResult = D3DReadFileToBlob(csoPath,
		shaderBlob.GetAddressOf());
	if (SUCCEEDED(readShaderResult))
	{
		DebugLogWrite("VertexShader.cso読み込み成功 / VertexShader.cso loaded successfully");
		return true;
	}
	DebugLogWrite("PixelShader.csoが無いためHLSLをコンパイル / Compile HLSL because PixelShader.cso is missing");
	ComPtr<ID3DBlob> errorBlob;
	HRESULT compileShaderResult = D3DCompileFromFile(
		hlslPath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
#ifdef _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
#endif
		0,
		shaderBlob.GetAddressOf(),
		errorBlob.GetAddressOf());
	if (FAILED(compileShaderResult))
	{
		if (errorBlob)
		{
			DebugLogWrite(static_cast<const char*>(errorBlob->GetBufferPointer()));
		}
		return false;
	}
	return true;
}

//---------------------------------------------------------
//シェーダー作成
//---------------------------------------------------------
bool SpriteRenderer::CreateShaders()
{
	ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();

	if (pDevice == nullptr)
	{
		DebugLogWrite("CreateShader時にDeviceがnullptr / Device is nullptr when calling CreateShader");
		return false;
	}

	//コンパイルの頂点シェーダーを受け取る
	ComPtr<ID3DBlob> vertexShaderBlob;

	if (!LoadVertexShaderBlob(
		L"Source/Shader/SpriteVertexShader.cso",
		L"Source/Shader/SpriteVertexShader.hlsl",
		vertexShaderBlob))
	{
		return false;
	}
	//コンパイル結果から頂点シェーダーを作成
	HRESULT createVertexShaderResult = pDevice->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf());

	if (FAILED(createVertexShaderResult))
	{
		DebugLogWrite("SpriteVertexShader作成失敗 / Failed to Create SpriteVertexShader");
		return false;
	}

	//InputLayout作成
	//頂点データの構造を設定
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0,  16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	 , 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//C++側の頂点とHLSL側の頂点を関連付ける
	HRESULT createInputLayoutResult = pDevice->CreateInputLayout(
		inputElementDesc,
		ARRAYSIZE(inputElementDesc),
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		m_inputLayout.GetAddressOf());

	if (FAILED(createInputLayoutResult))
	{
		DebugLogWrite("Sprite用InputLayout作成失敗 / Failed to Create InputLayout for Sprite");
		return false;
	}

	//ピクセルシェーダーをコンパイル
	ComPtr<ID3DBlob> pixelShaderBlob;

	if (!LoadPixelShaderBlob(
		L"Source/Shader/SpritePixelShader.cso",
		L"Source/Shader/SpritePixelShader.hlsl",
		pixelShaderBlob))
	{
		return false;
	}

	//コンパイル結果からピクセルシェーダーを作成
	HRESULT createPixelShaderResult = pDevice->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(),
		nullptr,
		m_pixelShader.GetAddressOf());

	if (FAILED(createPixelShaderResult))
	{
		DebugLogWrite("SpritePixelShader作成失敗 / Failed to Create SpritePixelShader");
		return false;
	}


	DebugLogWrite("Spriteシェーダー作成成功 / Sprite Shader Created Successfully");
	return true;
}


//---------------------------------------------------------
//頂点バッファ作成
//---------------------------------------------------------
bool SpriteRenderer::CreateVertexBuffer()
{
	ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
	if (pDevice == nullptr)
	{
		DebugLogWrite("CreateVertexBuffer時にDeviceがnullptr / Device is nullptr when calling CreateVertexBuffer");
		return false;
	}

	//四角形は三角形2枚で
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(sSpriteVertex) * 6;
	//毎回画像の位置は大きさを書き換えるため、D3D11_USAGE_DYNAMICを使用
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT createVertexBufferResult = pDevice->CreateBuffer(
		&vertexBufferDesc,
		nullptr,
		m_vertexBuffer.GetAddressOf());

	if (FAILED(createVertexBufferResult))
	{
		DebugLogWrite("Sprite用のVertexBuffer作成失敗 / Failed to create a vertex buffer for a sprite");
		return false;
	}

	return true;
}

bool SpriteRenderer::CreateConstantBuffer()
{
	ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();

	if (pDevice == nullptr)
	{
		DebugLogWrite("CreateConstantBuffer時にDeviceがnullptr / Device is nullptr when calling CreateConstantBuffer");
		return false;
	}

	D3D11_BUFFER_DESC constantBufferDesc = {};
	//HLSLに毎期あ転送するため、DYNAMICにする
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//ConstantBufferDataのサイズ
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	//定数バッファとして使用
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//CPUから書き換えられるようにする
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT createConstantBufferResult = pDevice->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		m_constantBuffer.GetAddressOf());

	if (FAILED(createConstantBufferResult))
	{
		DebugLogWrite("Sprite様ConstantBuffer作成失敗 / Failed to create ConstantBuffer for Sprite");
		return false;
	}

	return true;
}

bool SpriteRenderer::CreateSamplerState()
{
	ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();

	if (pDevice == nullptr)
	{
		DebugLogWrite("CreateSamplerState時にDeviceがnullptr / Device is nullptr when calling CreateSamplerState");
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc = {};

	//画像を拡大縮小した時に周囲の画素を補間して滑らかに表示する設定
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//UVが0~1の範囲を超えた場合、画像の端の色を使用する
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT createSamplerResult = pDevice->CreateSamplerState(
		&samplerDesc,
		m_samplerState.GetAddressOf());

	if (FAILED(createSamplerResult))
	{
		DebugLogWrite("Sprite用SamplerState作成失敗 / Failed to create a SamplerState for Sprite");
		return false;
	}

	return true;
}

//---------------------------------------------------------
//ブレンドステート作成
//---------------------------------------------------------
bool SpriteRenderer::CreateBlendState()
{
	ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();

	if (pDevice == nullptr)
	{
		DebugLogWrite("CreateBlendState時にDeviceがnullptr / Device is nullptr when calling CreateBlendState");
		return false;
	}

	D3D11_BLEND_DESC alphaBlendDesc = {};

	//透明度を使った描画を有効にする
	alphaBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	//最終色の計算
	//画像の色 x 画像の透明度 + 背景の色 x ( 1 - 画像の透明度 )
	alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT createAlphaBlendStateResult = pDevice->CreateBlendState(
		&alphaBlendDesc,
		m_alphaBlendState.GetAddressOf());

	if (FAILED(createAlphaBlendStateResult))
	{
		DebugLogWrite("Sprite用AlphaBlendState作成失敗 / Failed to Create an AlphaBlendState for a Sprite");
		return false;
	}

	//加算合成
	D3D11_BLEND_DESC additiveBlendDesc = {};
	additiveBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT createAdditiveBlendStateResult = pDevice->CreateBlendState(
			&additiveBlendDesc,
			m_additiveBlendState.GetAddressOf());

	if (FAILED(createAdditiveBlendStateResult))
	{
		DebugLogWrite(
			"Sprite用AdditiveBlendState作成失敗 / Failed to create an AdditiveBlendState for a sprite");
		return false;
	}
	return true;
}

bool SpriteRenderer::CreateDepthStencilState()
{
	ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
	if (pDevice == nullptr)
	{
		DebugLogWrite("CreateDepthStencilState時にDeviceがnullptr / Device is nullptr when calling CreateDepthStencilState");
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	//深度テストをしない
	//前後お関係は描画した順番で決める(後に書いたものが手前)
	depthStencilDesc.DepthEnable = FALSE;
	//深度を書き込まない
	//透明部分が「見えない壁」になるのを防ぐ
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	//ステンシル(型抜き)も使わない
	depthStencilDesc.StencilEnable = FALSE;

	HRESULT createDepthStencilStateResult = pDevice->CreateDepthStencilState(
		&depthStencilDesc,
		m_depthStencilState.GetAddressOf());

	if (FAILED(createDepthStencilStateResult))
	{
		DebugLogWrite("Sprite用DepthStencilState作成失敗 / Failed to create a DepthStencilState for Sprite");
		return false;
	}
	return true;
}

//---------------------------------------------------------
//画像描画
//---------------------------------------------------------
void SpriteRenderer::Draw(Texture* pTexture, float x, float y, float width, float height,const DirectX::XMFLOAT4& color,
	eSpriteBlendMode blendMode)
{
	if (pTexture == nullptr)
	{
		DebugLogWrite("SpriteRenderer::DrawのTextureがnullptr");
		return;
	}

	ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();

	if (pContext == nullptr)
	{
		DebugLogWrite("SpriteRenderer::Draw時にContextがnullptr");
		return;
	}

	//ピクセル座標をDirectXの座標へ変換する
	float left = (x / static_cast<float>(m_screenWidth)) * 2.0f - 1.0f;
	float right = ((x + width) / static_cast<float>(m_screenWidth)) * 2.0f - 1.0f;
	float top = 1.0f - (y / static_cast<float>(m_screenHeight)) * 2.0f;
	float bottom = 1.0f - ((y + height) / static_cast<float>(m_screenHeight)) * 2.0f;

	//四角形を構成する6頂点
	sSpriteVertex vertices[6] =
	{
		{{left  ,	top,0.0f, 1.0f},{1.0f,1.0f,1.0f,1.0f},{0.0f,0.0f}},
		{{right ,	top,0.0f, 1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,0.0f}},
		{{left  ,bottom,0.0f, 1.0f},{1.0f,1.0f,1.0f,1.0f},{0.0f,1.0f}},

		{{left  ,bottom,0.0f, 1.0f},{1.0f,1.0f,1.0f,1.0f},{0.0f,1.0f}},
		{{right ,   top,0.0f, 1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,0.0f}},
		{{right ,bottom,0.0f, 1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f}},
	};

	//頂点バッファを置き換え可能な状態にする
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	HRESULT mapVertexBufferResult = pContext->Map(
		m_vertexBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);

	if (FAILED(mapVertexBufferResult))
	{
		DebugLogWrite("Sprite頂点バッファのMap失敗 / SpriteVertexBuffer Mapping Failure");
		return;
	}

	//頂点情報をGPUにコピー
	memcpy(mappedResource.pData, vertices, sizeof(vertices));

	//頂点バッファの書き換えを終了
	pContext->Unmap(m_vertexBuffer.Get(), 0);

	//頂点バッファをDirectXに設定
	UINT stride = sizeof(sSpriteVertex);
	UINT offset = 0;

	ID3D11Buffer* vertexBuffer = m_vertexBuffer.Get();
	pContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//頂点の構造を設定
	pContext->IASetInputLayout(m_inputLayout.Get());
	//頂点を三角形として描画する
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//定数バッファへ送るデータを作る
	ConstantBufferData constantBufferData = {};

	//描画時に指定された色
	constantBufferData.vertexColor = color;

	XMMATRIX identityMatrix = XMMatrixIdentity();

	XMStoreFloat4x4(&constantBufferData.matrixTex, identityMatrix);
	XMStoreFloat4x4(&constantBufferData.matrixProj, identityMatrix);
	XMStoreFloat4x4(&constantBufferData.matrixWorld, identityMatrix);

	//定数バッファを書き換える
	D3D11_MAPPED_SUBRESOURCE mappedConstantBuffer = {};

	HRESULT mapConstantBufferResult =
		pContext->Map(m_constantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedConstantBuffer);
	if (FAILED(mapConstantBufferResult))
	{
		DebugLogWrite("Sprite定数バッファのMap失敗 / Sprite Constant Buffer Map Failure");
		return;
	}

	memcpy(mappedConstantBuffer.pData,
		&constantBufferData,
		sizeof(constantBufferData));

	pContext->Unmap(m_constantBuffer.Get(), 0);

	//シェーダーを設定
	pContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//HLSLのregister(b0)に設定する
	ID3D11Buffer* constantBuffer = m_constantBuffer.Get();

	pContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	//描画する画像をピクセルシェーダーに設定
	pTexture->SetPS(0);

	//テクスチャの読み取り方法を設定
	ID3D11SamplerState* samplerState = m_samplerState.Get();

	pContext->PSSetSamplers(0, 1, &samplerState);

	ID3D11BlendState* blendState = nullptr;
	switch (blendMode)
	{
	case eSpriteBlendMode::ADDITIVE:
	{
		//レーザー、発光花火など
		blendState = m_additiveBlendState.Get();
		break;
	}
	case eSpriteBlendMode::ALPHA:
	{

	}
	default:
	{
		//UI、通常画像、煙など
		blendState = m_alphaBlendState.Get();
		break;
	}
	}

	//透過を有効に
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
	pContext->OMSetBlendState(m_alphaBlendState.Get(), blendFactor, 0xffffffff);
	//深度テストを無効にする
	pContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
	//6頂点を描画
	pContext->Draw(6, 0);
	//次の3D描画などへ影響を残さないようにブレンドステートを解除
	pContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	pContext->OMSetDepthStencilState(nullptr, 0);
}

//---------------------------------------------------------
//終了処理
//---------------------------------------------------------
void SpriteRenderer::UnInitialize()
{
	m_alphaBlendState.Reset();
	m_samplerState.Reset();
	m_vertexBuffer.Reset();
	m_constantBuffer.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_vertexShader.Reset();
	m_depthStencilState.Reset();
	m_additiveBlendState.Reset();
	m_alphaBlendState.Reset();

	DebugLogWrite("SpriteRenderer終了処理完了 / SpriteRenderer Cleanup Complete");
}