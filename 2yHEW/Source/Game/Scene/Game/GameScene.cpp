#include "GameScene.h"

#include "../../../Framework/Input/input.h"
#include "../../Object/SpriteObject/test/test.h"
#include "../../Object/SpriteObject/player/Player.h"
#include "../../Component/ShowObject/CShowObject.h"
#include "../../Component/KeyMove/CKeyMove.h"

#include "../../../Framework/DebugSystem/DebugLog.h"
#ifdef _DEBUG
#include "../../../../thirdParty/imgui/include/imgui.h"
#endif

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
bool GameScene::Init()
{
	//GameSceneの初期化が通っているか確認
	//DebugLogWrite("GameScene初期化 / GameScene Init");

	//追加した順番に描画される(後の者が手前)
	AddGameObject<test>();
	AddGameObject<Player>();

	return InitGameObjects();
}

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void GameScene::Update()
{
	UpdateGameObjects();

	//エンターキーでタイトルに戻る
	if (m_input.GetKeyTrigger(VK_RETURN))
	{
		ChangeScene(TITLE);
	}
}

//---------------------------------------------------------
//描画
//---------------------------------------------------------
void GameScene::Draw()
{
	DrawGameObjects();
}

//---------------------------------------------------------
//終了
//---------------------------------------------------------
void GameScene::UnInit()
{
	//配列が解放される前に、参照を消しておく
	m_pTestSpriteObject = nullptr;

	//オブジェクトの解放は基底クラスに任せる
	SceneBase::UnInit();
}