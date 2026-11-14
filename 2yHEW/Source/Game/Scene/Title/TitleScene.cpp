#include "TitleScene.h"
#include "../../../Framework/Input/input.h"
#include "../../../Framework/DebugSystem/DebugLog.h"

bool TitleScene::Init()
{
    //TitleSceneの初期化が通っているか確認
    //DebugLogWrite("TitleScene初期化 / TitleScene Init");

    //タイトル画像のオブジェクトを追加
    //AddGameObject<TitleLogo>();
    return InitGameObjects();
}

void TitleScene::Update()
{
    UpdateGameObjects();

    //エンターキーでゲームシーンへ
    if (m_input.GetKeyTrigger(VK_RETURN))
    {
        ChangeScene(GAME);
    }
}

void TitleScene::Draw()
{
    DrawGameObjects();
}