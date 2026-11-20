#include "GameManager.h"

#include "../../Framework/Renderer/SpriteRenderer.h"
#include "../../Framework/Texture/TextureManager.h"
#include "../../Framework/DebugSystem/DebugLog.h"

#include "../Scene/Title/TitleScene.h"
#include "../Scene/Game/GameScene.h"

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
bool GameManager::Init()
{
	DebugLogWrite("Gameクラス初期化開始 / GameInit start");

	const UINT SCREEN_WIDTH = 1280;
	const UINT SCREEN_HEIGHT = 720;

	// 2D画像描画機能を初期化
	if (!SpriteRenderer::GetInstance().Initialize(SCREEN_WIDTH,SCREEN_HEIGHT))
	{
		DebugLogWrite("SpriteRenderer初期化失敗");
		return false;
	}

	//最初のシーン
	if (!ChangeScene(TITLE))
	{
		return false;
	}

	DebugLogWrite("Gameクラス初期化成功 / GameInit Suscess");

	return true;
}

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void GameManager::Update()
{
	m_input.Update();

	if (!m_currentScene)
	{
		return;
	}

	m_currentScene->Update();

	//シーンのUpdateが終わった後に切り替える
	//Update途中で切り替えると、実行中のシーンが自分自身を削除することになり、落ちる原因になるため
	eScene nextScene = m_currentScene->GetNextScene();

	if (nextScene != NONE)
	{
		if (!ChangeScene(nextScene))
		{
			//失敗したら今のシーンを続けて、依頼を取り消す
			m_currentScene->ResetNextScene();
		}
	}
}

//---------------------------------------------------------
//描画
//---------------------------------------------------------
void GameManager::Draw()
{
	if (m_currentScene)
	{
		m_currentScene->Draw();
	}
}

//---------------------------------------------------------
// //終了
//---------------------------------------------------------
void GameManager::Exit()
{
	if (m_currentScene)
	{
		m_currentScene->UnInit();
		m_currentScene.reset();
	}

	// TextureManagerが管理するTextureを解放する
	TextureManager::GetInstance().ClearTexture();

	// SpriteRendererが持つDirectXリソースを解放する
	SpriteRenderer::GetInstance().UnInitialize();
}


std::unique_ptr<SceneBase> GameManager::CreateScene(eScene sceneType)
{
	switch (sceneType)
	{
	case TITLE:	return std::make_unique<TitleScene>(m_input);
	case GAME:	return std::make_unique<GameScene>(m_input);
	default: return nullptr;
	}
}

//---------------------------------------------------------
//シーン切り替え
//---------------------------------------------------------
bool GameManager::ChangeScene(eScene nextScene)
{
	std::unique_ptr<SceneBase> newScene = CreateScene(nextScene);

	if (!newScene)
	{
		DebugLogWrite("未対応のシーン / Unsupported scene");
		return false;
	}

	//先に新しいシーンを初期化
	//失敗したときに、今のシーンをそのまま続けられるようにするため
	if (!newScene->Init())
	{
		DebugLogWrite("シーン初期化失敗 / Failed to Initialize Scene");
		return false;
	}

	//古いシーンを終了して入れ替える
	if (m_currentScene)
	{
		m_currentScene->UnInit();
	}

	m_currentScene = std::move(newScene);
	
	return true;
}