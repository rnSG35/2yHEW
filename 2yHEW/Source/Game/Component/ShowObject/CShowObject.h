#pragma once
#include "../Base/ComponentBase.h"
class CShowObject :
    public ComponentBase
{
public:
    CShowObject() = default;
    ~CShowObject() override = default;

    //表示する
    void Show();

    //非表示にする
    void Hide();

    //表示状態を直接設定
    void SetVisible(bool isVisible);

    //現在表示する状態か取得
    bool IsVisible() const;

private:
    //true ：描画する
    //false：描画しない
    bool m_isVisible = true;
};

