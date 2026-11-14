#include "SceneBase.h"
#include "../../../Framework/DebugSystem/DebugLog.h"

//---------------------------------------------------------
//コンストラクタ
//---------------------------------------------------------
SceneBase::SceneBase(eScene sceneType, Input& input) : m_input(input),m_sceneType(sceneType)
{
}

bool SceneBase::Init()
{
	return false;
}

void SceneBase::Update()
{}

void SceneBase::Draw()
{}

//---------------------------------------------------------
//終了処理
//---------------------------------------------------------
void SceneBase::UnInit()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->UnInitialize();
	}

	//配列を空にしてオブジェクトを解放
}

bool SceneBase::InitGameObjects()
{
	for (auto& gameObject : m_gameObjects)
	{
		if (!gameObject->Initialize())
		{
			DebugLogWrite("GameObject初期化失敗 / Failed to Initialize GameObject");
			return false;
		}
	}
	return true;
}

void SceneBase::UpdateGameObjects()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->Update();
	}
}

void SceneBase::DrawGameObjects()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->Draw();
	}
}

//---------------------------------------------------------
//終了処理
//---------------------------------------------------------
