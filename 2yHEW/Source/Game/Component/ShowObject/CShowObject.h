#pragma once
#include "../Base/ComponentBase.h"
class CShowObject :
    public ComponentBase
{
public:
    CShowObject() = default;
    ~CShowObject() override = default;

    
    /// <summary>
    /// 表示する
    /// </summary>
    void Show();

    /// <summary>
    /// 非表示にする
    /// </summary>
    void Hide();

    /// <summary>
    /// 表示状態を反転
    /// </summary>
    void Toggle();

    /// <summary>
    /// 表示状態を直接設定
    /// </summary>
    /// <param name="isVisible">true:表示　false:非表示</param>
    void SetVisible(bool isVisible);

    /// <summary>
    /// 現在表示する状態か取得
    /// </summary>
    /// <returns>bool型　true:表示状態　false:非表示状態</returns>
    bool IsVisible() const;

private:
    //true ：描画する
    //false：描画しない
    bool m_isVisible = true;
};