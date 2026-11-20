#include "SpriteObjectBase.h"

#include "../../Component/ShowObject/CShowObject.h"
#include "../../Component/Transform/CTransform.h"
#include "../../../Framework/Renderer/SpriteRenderer.h"
#include "../../../Framework/Texture/Texture.h"
#include "../../../Framework/Texture/TextureManager.h"
#include "../../../Framework/DebugSystem/DebugLog.h"

//---------------------------------------------------------
//コンストラクタ
//---------------------------------------------------------
SpriteObjectBase::SpriteObjectBase(const std::string& filePath) : m_filePath(filePath)
{
	//画像表示には位置が必要なので先につけておく
	//OnInit()、シーンからもすぐにCTransformを使える
	AddComponent<CTransform>();
}

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
bool SpriteObjectBase::Initialize()
{
	if (m_filePath.empty())
	{
		DebugLogWrite("SpriteObjectBaseのファイルパスが空 / The file path for SpriteObjectBase is empty");
		return false;
	}

	//TextureManagerから画像を取得
	m_texture = TextureManager::GetInstance().LoadTexture(m_filePath);
	if (m_texture == nullptr)
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

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void SpriteObjectBase::Update()
{
	//先にコンポーネントを更新
	UpdateComponents();

	OnUpdate();
}

//---------------------------------------------------------
//描画
//---------------------------------------------------------
void SpriteObjectBase::Draw()
{
	//ShowObjectComponentが付いているか確認
	std::shared_ptr<CShowObject> showComponent = GetComponent<CShowObject>();

	//コンポーネントが存在して、かつ非表示にないいていたら描画しない
	if (showComponent != nullptr && !showComponent->IsVisible())
	{
		return;
	}

	if (m_texture == nullptr)
	{
		return;
	}

	////SpriteRendererにTexture、位置、サイズを渡して描画する
	//SpriteRenderer::GetInstance().Draw(m_texture.get(), m_x, m_y, m_width, m_height, m_color, m_blendMode);
	std::shared_ptr<CTransform> transform = GetComponent<CTransform>();

	Float3 position =
		transform ? transform->GetPosition() : FLOAT3_ZERO;

	SpriteRenderer::GetInstance().Draw(
		m_texture.get(),
		position.x,
		position.y,
		m_width,
		m_height,
		m_color,
		m_blendMode);

	OnDraw();
}

//---------------------------------------------------------
//位置設定
//---------------------------------------------------------
void SpriteObjectBase::SetPosition(float x, float y, float z)
{
	std::shared_ptr<CTransform> transform = GetComponent<CTransform>();

	if (transform)
	{
		transform->SetPosition(x, y, z);
	}
}

//---------------------------------------------------------
//表示サイズ設定
//---------------------------------------------------------
void SpriteObjectBase::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

//---------------------------------------------------------
//色設定、基本的にしなくていい
//---------------------------------------------------------
void SpriteObjectBase::SetColor(float red, float green, float blue, float alpha)
{
	m_color =
	{
		red,
		green,
		blue,
		alpha
	};
}

//---------------------------------------------------------
//ブレンドステートの型、合成方法設定
//---------------------------------------------------------
void SpriteObjectBase::SetBlendMode(eSpriteBlendMode blendMode)
{
	m_blendMode = blendMode;
}