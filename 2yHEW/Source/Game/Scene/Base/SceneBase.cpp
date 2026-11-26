#include "SceneBase.h"
#include "../../Component/Transform/CTransform.h"
#include <algorithm>
#include "../../../Framework/Renderer/SpriteRenderer.h"

#include "../../../Framework/DebugSystem/DebugLog.h"

namespace
{
	//オブジェクトの前後関係(z)を取得
	//CTransformがないオブジェクトはz=0として扱う
	float GetDrawDepth(GameObject* pGameObject)
	{
		std::shared_ptr<CTransform> transform = pGameObject->GetComponent<CTransform>();
		return transform ? transform->GetPosition().z : 0.0f;
	}
}

//---------------------------------------------------------
//コンストラクタ
//---------------------------------------------------------
SceneBase::SceneBase(eScene sceneType, const wchar_t* sceneName, Input& input) : m_input(input), m_sceneType(sceneType), m_sceneName(sceneName)
{
	//カメラに移す範囲を、画面の大きさに合わせる
	m_camera.SetViewSize(
		static_cast<float>(SpriteRenderer::GetInstance().GetScreenWidth()),
		static_cast<float>(SpriteRenderer::GetInstance().GetScreenHeight()));
}

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
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

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void SceneBase::UpdateGameObjects()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->Update();
	}
}

//---------------------------------------------------------
//描画
//---------------------------------------------------------
void SceneBase::DrawGameObjects()
{
	//このシーンのカメラで描画する
	SpriteRenderer::GetInstance().SetViewProjection(m_camera.GetViewProjectionMatrix());

	m_drawObjects.clear();

	for (auto& gameObject : m_gameObjects)
	{
		m_drawObjects.push_back(gameObject.get());
	}

	//奥にあるもの(zが大きい物)から描画
	//SpriteRendererは深度テストを切っているので、後から書いたものが手前に描画される
	//stable_sortを使うのは、zが同じオブジェクト同士では今まで通りの「追加した順番」を保つため
	// [](引数){ 処理 } はラムダ式(その場で作る小さな関数)
	// stable_sortは「2つを比べてどちらを先にするか」を
	// この関数で判断する。trueならpLeftが先に描画される
	std::stable_sort(m_drawObjects.begin(), m_drawObjects.end(),[]( GameObject* pLeft, GameObject* pRight)
		{
			return GetDrawDepth(pLeft) > GetDrawDepth(pRight);
		});

	for (GameObject* pGameObject : m_drawObjects)
	{
		pGameObject->Draw();
	}

}

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
	m_gameObjects.clear();
	//描画用の配列も空にして解放
	m_drawObjects.clear();
}