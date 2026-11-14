#pragma once
#include "../Base/SceneBase.h"
class TitleScene :
    public SceneBase
{
public:
    //TITLEシーンだと基底クラスに伝える
    TitleScene(Input& input) : SceneBase(TITLE, input) {}

    bool Init() override;
    void Update() override;
    void Draw() override;
};