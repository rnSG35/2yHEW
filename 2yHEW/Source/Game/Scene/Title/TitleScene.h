#pragma once
#include "../Base/SceneBase.h"
class TitleScene :
	public SceneBase
{
public:
	/// <summary>
	/// TITLEシーンだと基底クラスに伝える
	/// </summary>
	/// <param name="input">Gameが持つ入力</param>
	TitleScene(Input& input) : SceneBase(TITLE, L"Title", input) {}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// 終了処理
	/// </summary>
	void UnInit() override;
};