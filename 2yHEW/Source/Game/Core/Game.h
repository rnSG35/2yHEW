#pragma once
//#include "Input/Input.h"

/*
* @brief ゲームのコアクラス
* @details シーンの更新などゲームループを行う
*/
class Game
{
public:
	Game() {}
	~Game() {}
	// 初期化
	void Init();
	// 更新
	void Update(const float deltaTime);
	// 描画
	void Draw();
	// 終了
	void Exit();
private:
//	Input m_input;
};