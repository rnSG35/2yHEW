#include "SpriteObjectBase.h"

#include "../../Framework/Renderer/SpriteRenderer.h"
#include "../../Framework/Texture/Texture.h"
#include "../../Framework/Texture/TextureManager.h"
#include "../../Framework/DebugSystem/DebugLog.h"

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
bool SpriteObjectBase::Init()
{
	if (m_filePath.empty())
	{
		DebugLogWrite("SpriteObjectBaseのファイルパスが空 / The file path for SpriteObjectBase is empty");
		return false;
	}

	//TextureManagerから画像を取得
	m_texture = TextureManager::GetInstance().LoadTexture(m_filePath);
	if (!m_texture)
	{
		DebugLogWrite("SpriteObjectBaseの画像読み込み失敗 / SpriteObjectBase image load failure");
		return false;
	}

	//サイズが設定されていなければ、読み込んだ画像本来のサイズを使用する
	if (m_width <= 0.0f)
	{
		m_width = static_cast<float>(m_texture->GetWidth());
	}
	if (m_height <= 0.0f)
	{
		m_height = static_cast<float>(m_texture->GetHeight());
	}

	OnInit();
	
	return true;
}

void SpriteObjectBase::Update()
{
	OnUpdate();
}

void SpriteObjectBase::Draw()
{
	if (!m_texture)
	{
		return;
	}

	//SpriteRendererにTexture、位置、サイズを渡して描画する
	SpriteRenderer::GetInstance().Draw(m_texture.get(), m_x, m_y, m_width, m_height);

	OnDraw();
}

//---------------------------------------------------------
//表示位置設定
//---------------------------------------------------------
void SpriteObjectBase::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

//---------------------------------------------------------
//表示サイズ設定
//---------------------------------------------------------
void SpriteObjectBase::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;
}
