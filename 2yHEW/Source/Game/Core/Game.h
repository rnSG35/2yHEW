

#pragma once

#include "../../Framework/Input/Input.h"
#include "../Object/GameObject.h"

#include <memory>
#include <vector>
#include <concepts>

class Game
{
public:

	Game() = default;
	~Game() = default;

	bool Init();
	void Update();
	void Draw();
	void Exit();

private:

	Input m_input;

	//基底クラス(GameObject)のポインタで持つと、オブジェクトを同じ配列に入れられる
	//unique_ptrは配列から消えると自動で解放される
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;

	//オブジェクトを生成して配列に追加
	//戻り値は追加したオブジェクトのポインタ
	template<std::derived_from<GameObject> T>
	T* AddGameObject()
	{
		std::unique_ptr<T> gameObject = std::make_unique<T>();

		//配列に移す前にポインタを取っておく
		//(moveした後はgameObjectが空になるため)
		T* gameObjectPointer = gameObject.get();

		m_gameObjects.push_back(std::move(gameObject));

		return gameObjectPointer;
	}

	//表示確認用オブジェクト
	GameObject*	m_testSpriteObject = nullptr;
};