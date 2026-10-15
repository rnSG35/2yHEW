#include "SpriteObjectBase.h"

#include "../../Framework/Texture/Texture.h"
#include "../../Framework/Texture/TextureManager.h"
#include "../../Framework/DebugSystem/DebugLog.h"

void SpriteObjectBase::Init()
{
	if (!m_filePath.empty())
	{
		m_texture = TextureManager::GetInstance().LoadTexture(m_filePath);

		if (!m_texture) { DebugLogWrite("Textureì«Ç›çûÇ›é∏îs / Failed to load texture"); }
	}
	OnInit();
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
	

	OnDraw();
}