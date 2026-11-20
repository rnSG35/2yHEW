

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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 終了
	/// </summary>
	void Exit();

	/// <summary>
	/// 今のシーンからゲーム終了が依頼されているか
	/// シーンがないときはfalse、m_currentSceneが空のままだと落ちるため、先に確認
	/// </summary>
	/// <returns>動いているシーンと、動いているシーンにゲーム終了が依頼されているか</returns>
	bool IsQuitRequested() const
	{
		return m_currentScene && m_currentScene->IsQuitRequested();
	}

	//今のシーン名を取得
	//シーンがないときは"None"を返す、nullptrで返すと、表示する側で落ちる原因になる
	const wchar_t* GetCurrentSceneName() const
	{
		return m_currentScene ? m_currentScene->GetSceneName() : L"None";
	}
private:
	/// <summary>
	/// ゲームの入力
	/// </summary>
	Input m_input;

	//動いているシーン
	std::unique_ptr<SceneBase> m_currentScene;

	/// <summary>
	/// eSceneから、対応するシーンを作る、新しくシーンを増やしたらcaseを追加
	/// </summary>
	/// <param name="sceneType">新しく作るシーン</param>
	/// <returns></returns>
	std::unique_ptr<SceneBase> CreateScene(eScene sceneType);

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="nextScene">次のシーン</param>
	/// <returns>true:成功　false:失敗</returns>
	bool ChangeScene(eScene nextScene);
};