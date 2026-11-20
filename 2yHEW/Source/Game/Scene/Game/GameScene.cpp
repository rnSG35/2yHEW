#include "GameScene.h"

#include "../../../Framework/Input/input.h"
#include "../../Object/SpriteObject/test/test.h"
#include "../../Object/SpriteObject/laser/laser.h"
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
	m_pTestSpriteObject = AddGameObject<test>();
	AddGameObject<laser>();


	m_pTest = AddGameObject<Player>();

	//キー入力で動かせるようにする
	m_pTest->AddComponent<CKeyMove>(m_input);

	return InitGameObjects();
}

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void GameScene::Update()
{
	UpdateGameObjects();

	//スペースキーで表示を切り替える
	if (m_pTestSpriteObject && m_input.GetKeyTrigger(VK_SPACE))
	{
		std::shared_ptr<CShowObject> showComponent = m_pTestSpriteObject->GetComponent<CShowObject>();

		if (showComponent)
		{
			showComponent->Toggle();
		}
	}

	//エンターキーでタイトルに戻る
	if (m_input.GetKeyTrigger(VK_RETURN))
	{
		ChangeScene(TITLE);
	}


#ifdef _DEBUG
	//デバッグ用画面
	ImGui::Begin("GameScene Debug");

	//「表示」をUTF-8の値で直接書いたもの
	//ソースの文字コードに関係なく、必ずUTF-8になる
	ImGui::Text("\xE8\xA1\xA8\xE7\xA4\xBA");
	//フレームレートを表示
	ImGui::Text("FPS:%.2f", ImGui::GetIO().Framerate);

	//testの表示・非表示を切り替え
	if (m_pTestSpriteObject)
	{
		std::shared_ptr<CShowObject> showComponent = m_pTestSpriteObject->GetComponent<CShowObject>();

		if (showComponent)
		{
			//チェックボックスはboolの変数を書き換えるので、一度コピーする
			bool isVisible = showComponent->IsVisible();

			//チェックが変わった時だけtrueになる
			if (ImGui::Checkbox("test表示", &isVisible))
			{
				showComponent->SetVisible(isVisible);
			}
		}
	}
	ImGui::End();
#endif
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