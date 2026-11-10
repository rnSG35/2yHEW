#include "Game.h"

#include "../../Framework/Renderer/SpriteRenderer.h"
#include "../../Framework/Texture/TextureManager.h"
#include "../../Framework/DebugSystem/DebugLog.h"
#include "../../Game/Object/SpriteObject/test/test.h"
#include "../../Game/Object/SpriteObject/laser/laser.h"

#include "../Component/ShowObject/CShowObject.h"

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
bool Game::Init()
{
	DebugLogWrite("Gameクラス初期化開始 / GameInit start");

	const UINT SCREEN_WIDTH = 1280;
	const UINT SCREEN_HEIGHT = 720;

	// 2D画像描画機能を初期化
	if (!SpriteRenderer::GetInstance().Initialize(
		SCREEN_WIDTH,
		SCREEN_HEIGHT))
	{
		DebugLogWrite(
			"SpriteRenderer初期化失敗");

		return false;
	}

	// テスト用画像オブジェクトを生成
	m_testSpriteObject = AddGameObject<test>();
	AddGameObject<laser>();
	

	for (auto& gameObject : m_gameObjects)
	{
		if (!gameObject->Initialize())
		{
			DebugLogWrite("GameObject初期化失敗 / GameObject Initialization Failed");
			return false;
		}
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

	//オブジェクトをまとめて更新
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->Update();
	}

	//スペースキーでtestの表示を切り替える
	if (m_testSpriteObject && m_input.GetKeyTrigger(VK_SPACE))
	{
		std::shared_ptr<CShowObject> showComponent = m_testSpriteObject->GetComponent<CShowObject>();

		if (showComponent)
		{
			showComponent->Toggle();
			//DebugLogWrite("CShowObjectの表示状態を切り替えました。");
		}
	}
}

//---------------------------------------------------------
// 描画
//---------------------------------------------------------
void Game::Draw()
{
	//オブジェクトをまとめて描画
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->Draw();
	}
}

//---------------------------------------------------------
// 終了
//---------------------------------------------------------
void Game::Exit()
{
	//オブジェクトをまとめて終了処理
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->UnInitialize();
	}

	//配列を空にして、全オブジェクトを解放
	//オブジェクトが持つTextureのshared_ptrを先に手放すため、TextureManagerより前に行う
	m_gameObjects.clear();
	m_testSpriteObject = nullptr;


	// TextureManagerが管理するTextureを解放する
	TextureManager::GetInstance()
		.ClearTexture();

	// SpriteRendererが持つDirectXリソースを解放する
	SpriteRenderer::GetInstance()
		.UnInitialize();
}