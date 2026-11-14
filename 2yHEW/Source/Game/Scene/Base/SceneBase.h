#pragma once
#include "../../Object/GameObject.h"

#include <memory>
#include <vector>
#include <concepts>

class Input;

//シーンの種類
enum eScene
{
	TITLE,
	GAME,
	RESULT,

	//切り替え依頼なし
	NONE
};

class SceneBase
{
public:
	//sceneType：自分がどのシーンか
	//input    ：Gameが持つ入力
	SceneBase(eScene sceneType, Input& input);

	//基底クラスのポインタで削除するので、デストラクタはvirtual
	virtual ~SceneBase() = default;

	//派生クラスで上書きする関数
	virtual bool Init();
	virtual void Update();
	virtual void Draw();
	virtual void UnInit();

	//現在のシーンを取得
	eScene GetSceneType() const { return m_sceneType; }

	//切り替え先のシーンを取得
	eScene GetNextScene() const { return m_nextScene; }

	//切り替え先の依頼を取り消す(切り替え失敗の対策)
	void ResetNextScene() { m_nextScene = NONE; }

protected:
	//入力(Gameが持っているもの)
	Input& m_input;

	//シーン切り替えの依頼、ここでは切り替えず、保存するだけ
	//実際の切り替えはUpdateが終わった後にGameが行う
	void ChangeScene(eScene nextScene) { m_nextScene = nextScene; }

	//オブジェクトを生成してシーンに追加
	template<std::derived_from<GameObject> T>
	T* AddGameObject()
	{
		std::unique_ptr<T> gameObject = std::make_unique<T>();

		//moveすると空になるので、先にポインタを取っておく
		T* gameObjectPointer = gameObject.get();

		m_gameObjects.push_back(std::move(gameObject));

		return gameObjectPointer;
	}

	//シーン内のオブジェクトをまとめてhそり
	//各シーンのInit,Update,Drawから呼ぶ
	bool InitGameObjects();
	void UpdateGameObjects();
	void DrawGameObjects();

private:
	//現在のシーン
	eScene m_sceneType;

	//切り替え先のシーン
	eScene m_nextScene = NONE;

	//シーンにおいてあるオブジェクト
	std::vector < std::unique_ptr<GameObject>> m_gameObjects;
};