#pragma once
#include "../Base/SceneBase.h"
class GameScene :
    public SceneBase
{
public:
    GameScene(Input& input) : SceneBase(GAME, input) {}

    bool Init() override;
    void Update() override;
    void Draw() override;
    void UnInit() override;

private:

    //スペースキーで表示を切り替えるオブジェクト
    GameObject* m_testSpriteObject = nullptr;
};