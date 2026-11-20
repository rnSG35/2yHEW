#pragma once
#include "../../Object/GameObject.h"

#include <memory>
#include <vector>
#include <concepts>

class Input;

/// <summary>
/// シーンの種類
/// </summary>
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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="sceneType">自分がどのシーンか</param>
	/// <param name="sceneName">キャプションなどに表示するシーン名</param>
	/// <param name="input">Gameが持つ入力</param>
	SceneBase(eScene sceneType, const wchar_t* sceneName, Input& input);

	//基底クラスのポインタで削除するので、デストラクタはvirtual
	virtual ~SceneBase() = default;

	//派生クラスで上書きする関数、純粋仮想関数にすることで派生クラスに実装を強制
	virtual bool Init() = 0;	
	virtual void Update() = 0;	
	virtual void Draw() = 0;	
	virtual void UnInit() = 0;	
	
	/// <summary>
	/// ゲーム終了が依頼されているか取得
	/// </summary>
	/// <returns>true:依頼あり　false:依頼なし</returns>
	bool IsQuitRequested() const { return m_isQuitRequested; }

	/// <summary>
	/// 現在のシーンを取得
	/// </summary>
	/// <returns>列挙型変数eSceneで管理している「現在のシーン」</returns>
	eScene GetSceneType() const { return m_sceneType; }

	/// <summary>
	/// 切り替え先のシーンを取得
	/// </summary>
	/// <returns>列挙型変数eSceneで管理している「現在のシーン」</returns>
	eScene GetNextScene() const { return m_nextScene; }

	/// <summary>
	/// シーン名を取得
	/// </summary>
	/// <returns>wchar_tで管理している「シーン名」</returns>
	const wchar_t* GetSceneName() const { return m_sceneName; }

	/// <summary>
	/// 切り替え先の依頼を取り消す(切り替え失敗の対策)
	/// </summary>
	void ResetNextScene() { m_nextScene = NONE; }

protected:
	//入力(Gameが持っているもの)
	Input& m_input;

	//ゲーム終了を依頼
	//依頼するだけ、実際はUpdate()が終わったあとにApplicationがやる
	void RequestQuit() { m_isQuitRequested = true; }

	/// <summary>
	/// シーン切り替えの依頼、ここでは切り替えず、保存するだけ
	/// 実際の切り替えはUpdateが終わった後にGameが行う
	/// </summary>
	/// <param name="nextScene">列挙型変数eSceneで管理している「シーン名」</param>
	void ChangeScene(eScene nextScene) { m_nextScene = nextScene; }

	/// <summary>
	/// オブジェクトを生成してシーンに追加
	/// </summary>
	/// <typeparam name="T">追加するオブジェクトのクラス名</typeparam>
	/// <returns></returns>
	template<std::derived_from<GameObject> T>
	T* AddGameObject()
	{
		std::unique_ptr<T> gameObject = std::make_unique<T>();

		//シーンが持っている入力をオブジェクトに渡す
		gameObject->m_pInput = &m_input;

		//moveすると空になるので、先にポインタを取っておく
		T* pGameObjectPointer = gameObject.get();

		m_gameObjects.push_back(std::move(gameObject));

		return pGameObjectPointer;
	}

	//シーン内のオブジェクトをまとめて処理
	//各シーンのInit,Update,Drawから呼ぶ
	bool InitGameObjects();
	void UpdateGameObjects();
	void DrawGameObjects();

private:
	//bool型依頼確認用(最初は依頼なしなのでfalse)
	bool m_isQuitRequested = false;

	//列挙型変数で管理している「現在のシーン」
	eScene m_sceneType;

	//列挙型変数、切り替え先のシーン
	eScene m_nextScene = NONE;

	//シーン名、プログラムが終わるまで残るので、コピーせずポインタで持つ
	const wchar_t* m_sceneName;

	//描画順に並べ替えるための配列、m_gameObjects自体を並べ替えると更新の順番まで変わるので、描画用の配列を作る
	//毎フレーム作り直すとメモリ確保が増えるので、メンバ変数にして使いまわす
	std::vector<GameObject*> m_drawObjects;

	//vector型シーンにおいてあるオブジェクト
	std::vector < std::unique_ptr<GameObject>> m_gameObjects;
};