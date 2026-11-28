#include "TitleScene.h"

#include "../../../Framework/Input/input.h"

#include "../../../Framework/DebugSystem/DebugLog.h"

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
bool TitleScene::Init()
{
	//TitleSceneの初期化が通っているか確認
	//DebugLogWrite("TitleScene初期化 / TitleScene Init");

	//タイトル画像のオブジェクトを追加
	//AddGameObject<TitleLogo>();
	return InitGameObjects();
}

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void TitleScene::Update()
{
	UpdateGameObjects();

	//エンターキーでゲームシーンへ
	if (m_input.GetKeyTrigger(VK_RETURN))
	{
		ChangeScene(GAME);
	}

	//エスケープキーでゲーム終了を依頼
	if (m_input.GetKeyTrigger(VK_ESCAPE))
	{
		//ここにループ終了の処理
		RequestQuit();
	}
}

//---------------------------------------------------------
//描画
//---------------------------------------------------------
void TitleScene::Draw()
{
	DrawGameObjects();
}

//---------------------------------------------------------
//終了処理
//---------------------------------------------------------
void TitleScene::UnInit()
{
	//タイトル画面固有の終了処理があればここ

	//オブジェクトの解放は基底クラスに任せる
	SceneBase::UnInit();
}