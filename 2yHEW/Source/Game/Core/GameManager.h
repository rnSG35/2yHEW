

#pragma once

#include "../../Framework/Input/Input.h"
#include "../Scene/Base/SceneBase.h"

//#include <memory>
//#include <vector>
//#include <concepts>

class GameManager
{
public:
	GameManager() = default;
	~GameManager() = default;

	bool Init();
	void Update();
	void Draw();
	void Exit();

	//今のシーンからゲーム終了が依頼されているか
	//シーンがないときはfalse、m_currentSceneが空のままだと落ちるため、先に確認
	bool IsQuitRequested() const
	{
		return m_currentScene && m_currentScene->IsQuitRestested();
	}

	//今のシーン名を取得
	//シーンがないときは"None"を返す、nullptrで返すと、表示する側で落ちる原因になる
	const wchar_t* GetCurrentSceneName() const
	{
		return m_currentScene ? m_currentScene->GetSceneName() : L"None";
	}
private:
	Input m_input;

	//動いているシーン
	std::unique_ptr<SceneBase> m_currentScene;

	//eSceneから、対応するシーンを作る
	std::unique_ptr<SceneBase> CreateScene(eScene sceneType);

	//シーンの切り替え
	bool ChangeScene(eScene nextScene);
};