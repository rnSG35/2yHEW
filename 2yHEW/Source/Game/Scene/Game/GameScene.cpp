#include "GameScene.h"

#include "../../../Framework/Input/input.h"
#include "../../../Framework/DebugSystem/DebugLog.h"

#include "../../Object/SpriteObject/test/test.h"
#include "../../Object/SpriteObject/laser/laser.h"
#include "../../Object/SpriteObject/player/Player.h"
#include "../../Component/ShowObject/CShowObject.h"

bool GameScene::Init()
{
    //GameSceneの初期化が通っているか確認
    //DebugLogWrite("GameScene初期化 / GameScene Init");

    //追加した淳に描画される(後の者が手前)
    m_testSpriteObject = AddGameObject<test>();
    AddGameObject<laser>();
    AddGameObject<Player>();

    return InitGameObjects();
}

void GameScene::Update()
{
    UpdateGameObjects();

    //スペースキーで表示を切り替える
    if (m_testSpriteObject && m_input.GetKeyTrigger(VK_SPACE))
    {
        std::shared_ptr<CShowObject> showComponent = m_testSpriteObject->GetComponent<CShowObject>();
        
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
}

void GameScene::Draw()
{
    DrawGameObjects();
}

void GameScene::UnInit()
{
    //配列が解放される前に、参照を消しておく
    m_testSpriteObject = nullptr;

    //オブジェクトの解放は基底クラスに任せる
    SceneBase::UnInit();
}