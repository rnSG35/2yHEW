//#pragma once
////#include "Input/Input.h"
//
///*
//* @brief ゲームのコアクラス
//* @details シーンの更新などゲームループを行う
//*/
//class Game
//{
//public:
//	Game() {}
//	~Game() {}
//	// 初期化
//	void Init();
//	// 更新
//	void Update(/*const float deltaTime*/);
//	// 描画
//	void Draw();
//	// 終了
//	void Exit();
//private:
////	Input m_input;
//};

#pragma once

#include <memory>
#include "../../Game/SpriteObject/test/test.h"

class test;

class Game
{
public:

	Game() = default;
	~Game() = default;

	bool Init();
	void Update();
	void Draw();
	void Exit();

private:

	//---------------------------------------------------------
	// 表示確認用オブジェクト
	//---------------------------------------------------------
	std::unique_ptr<test>
		m_testSpriteObject;
};