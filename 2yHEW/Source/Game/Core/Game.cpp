#include "Game.h"

#include "../../Framework/Renderer/SpriteRenderer.h"
#include "../../Game/Object/SpriteObject/test/test.h"
#include "../../Framework/Texture/TextureManager.h"
//#include "../../Framework/DebugSystem/DebugLog.h"
#include "DebugLog.h"

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
bool Game::Init()
{
	DebugLogWrite("Gameクラス初期化開始 / GameInit start");

	const UINT SCREEN_WIDTH = 1280;
	const UINT SCREEN_HEIGHT = 720;

	//---------------------------------------------------------
	// 2D画像描画機能を初期化
	//---------------------------------------------------------
	if (!SpriteRenderer::GetInstance().Initialize(
		SCREEN_WIDTH,
		SCREEN_HEIGHT))
	{
		DebugLogWrite(
			"SpriteRenderer初期化失敗");

		return false;
	}

	//---------------------------------------------------------
	// テスト用画像オブジェクトを生成
	//---------------------------------------------------------
	m_testSpriteObject =
		std::make_unique<test>();

	if (!m_testSpriteObject->Initialize())
	{
		DebugLogWrite(
			"TestSpriteObject初期化失敗");

		return false;
	}

	DebugLogWrite("Gameクラス初期化成功 / GameInit Suscess");

	return true;
}

//---------------------------------------------------------
// 更新
//---------------------------------------------------------
void Game::Update()
{
	m_input.Update();

	if (m_testSpriteObject)
	{
		m_testSpriteObject->Update();
	}

	if (m_input.GetKeyTrigger(VK_SPACE))
	{
		
	}
}

//---------------------------------------------------------
// 描画
//---------------------------------------------------------
void Game::Draw()
{
	if (m_testSpriteObject)
	{
		m_testSpriteObject->Draw();
	}
}

//---------------------------------------------------------
// 終了
//---------------------------------------------------------
void Game::Exit()
{
	//---------------------------------------------------------
	// オブジェクトが持つshared_ptrを先に解放する
	//---------------------------------------------------------
	m_testSpriteObject.reset();

	//---------------------------------------------------------
	// TextureManagerが管理するTextureを解放する
	//---------------------------------------------------------
	TextureManager::GetInstance()
		.ClearTexture();

	//---------------------------------------------------------
	// SpriteRendererが持つDirectXリソースを解放する
	//---------------------------------------------------------
	SpriteRenderer::GetInstance()
		.UnInitialize();
}