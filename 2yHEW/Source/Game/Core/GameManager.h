

#pragma once

#include "../../Framework/Input/Input.h"
#include "../Scene/Base/SceneBase.h"

#include <memory>
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

private:

	Input m_input;

	//動いているシーン
	std::unique_ptr<SceneBase> m_currentScene;

	//eSceneから、対応するシーンを作る
	std::unique_ptr<SceneBase> CreateScene(eScene sceneType);

	//シーンの切り替え
	bool ChangeScene(eScene nextScene);


};