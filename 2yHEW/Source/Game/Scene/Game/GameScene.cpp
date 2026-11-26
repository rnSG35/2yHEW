#include "GameScene.h"

#include "../../../Framework/Input/input.h"
#include "../../Object/SpriteObject/test/test.h"
#include "../../Object/SpriteObject/player/Player.h"
#include "../../Component/ShowObject/CShowObject.h"
#include "../../Component/KeyMove/CKeyMove.h"
#include "../../Component/Transform/CTransform.h"

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
	m_pPlayer = AddGameObject<Player>();

	return InitGameObjects();
}

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void GameScene::Update()
{
	UpdateGameObjects();

	//---------------------------------------------------------
//カメラにプレイヤーを追わせる
//
//0.1は「1フレームで残りの距離の1割だけ近づく」という意味
//すぐ追いつくより、少し遅れて追う方が画面の動きが滑らかに見える
//---------------------------------------------------------
	if (m_pPlayer)
	{
		std::shared_ptr<CTransform> transform = m_pPlayer->GetComponent<CTransform>();

		if (transform)

		{
			const Float3& playerPosition = transform->GetPosition();
			m_camera.Follow(Float2(playerPosition.x, playerPosition.y), 0.1f);
		}
	}

	//エンターキーでタイトルに戻る
	if (m_input.GetKeyTrigger(VK_O))
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
	m_pPlayer = nullptr;
	//オブジェクトの解放は基底クラスに任せる
	SceneBase::UnInit();
}