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
	AddComponent<CShowObject>();
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
	if (m_size.x <= 0.0f)
	{
		m_size.x = static_cast<float>(m_texture->GetWidth());
	}
	if (m_size.y <= 0.0f)
	{
		m_size.y = static_cast<float>(m_texture->GetHeight());
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

	//コンポーネントが存在して、かつ非表示になっていたら描画しない
	if (showComponent != nullptr && !showComponent->IsVisible())
	{
		return;
	}

	if (m_texture == nullptr)
	{
		return;
	}

	//SpriteRendererにTexture、位置、サイズを渡して描画する
	std::shared_ptr<CTransform> transform = GetComponent<CTransform>();

	Float3 position = transform ? transform->GetPosition() : FLOAT3_ZERO;
	Float3 rotation = transform ? transform->GetRotation() : FLOAT3_ZERO;

	SpriteRenderer::GetInstance().Draw(
		m_texture.get(),
		position.x,
		position.y,
		m_size.x,
		m_size.y,
		m_color,
		m_blendMode,
		rotation.z);

	OnDraw();
}

void SpriteObjectBase::UnInitialize()
{
	//派生クラスが画像などを使う処理を書いても動くように、解放より前に書く
	OnUnInit();

	//画像の参照を手放す、画像そのものはTextureManagerが解放する
	m_texture.reset();
}

//---------------------------------------------------------
//位置設定(Float3で指定)
//---------------------------------------------------------
void SpriteObjectBase::SetPosition(const Float3& position)
{
	std::shared_ptr<CTransform> transform = GetComponent<CTransform>();

	if (transform)
	{
		transform->SetPosition(position);
	}
}

//---------------------------------------------------------
//位置設定(数値で指定)
//---------------------------------------------------------
void SpriteObjectBase::SetPosition(float x, float y, float z)
{
	//中身はFloat3のほうに任せる
	SetPosition(Float3(x, y, z));
}

void SpriteObjectBase::SetPosition(const Float2& position, float z)
{
	SetPosition(Float3(position.x, position.y, z));
}

//---------------------------------------------------------
//表示サイズ設定(Float2で指定)
//---------------------------------------------------------
void SpriteObjectBase::SetSize(const Float2& size)
{
	m_size = size;
}

//---------------------------------------------------------
//表示サイズ設定(数値で指定)
//---------------------------------------------------------
void SpriteObjectBase::SetSize(float width, float height)
{
	m_size = Float2(width, height);
}

//---------------------------------------------------------
//色設定(Float4で指定)
//---------------------------------------------------------
void SpriteObjectBase::SetColor(const Float4& color)
{
	m_color = color;
}

//---------------------------------------------------------
//色設定(数値で指定)
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