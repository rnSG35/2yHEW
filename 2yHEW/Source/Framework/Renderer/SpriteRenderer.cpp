#include "SpriteRenderer.h"

#include "../Graphics/Graphics.h"
#include "../Texture/Texture.h"
#include "../DebugSystem/DebugLog.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

//---------------------------------------------------------
//スプライト用頂点
//---------------------------------------------------------
namespace
{
	struct sSpriteVertex
	{
		//画面上の頂点位置
		float position[3];

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
}

void SpriteRenderer::Draw(Texture* texture, float x, float y, float width, float height)
{
}
bool SpriteRenderer::CreateShaders()
{
	return false;
}

bool SpriteRenderer::CreateVertexBuffer()
{
	return false;
}

bool SpriteRenderer::CreateBlendState()
{
	return false;
}
