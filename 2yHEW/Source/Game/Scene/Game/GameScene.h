#pragma once
#include "../Base/SceneBase.h"
class GameScene :
	public SceneBase
{
public:
	/// <summary>
	/// GAMEシーンだと基底クラスに伝える
	/// </summary>
	/// <param name="input">Gameが持つ入力</param>
	GameScene(Input& input) : SceneBase(GAME, L"Game", input) {}

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
	/// 終了
	/// </summary>
	void UnInit() override;

private:
	GameObject* m_pPlayer = nullptr;
};