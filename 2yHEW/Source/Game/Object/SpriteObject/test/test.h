#pragma once
#include "../SpriteObjectBase.h"
class test :
    public SpriteObjectBase
{
public:
    test();
    ~test() override = default;

private:
    void OnInit() override;
    void OnUpdate() override;
    void OnDraw() override;
};

