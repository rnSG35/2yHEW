

#pragma once

#include "Input.h"

#include <memory>
#include "test/test.h"

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

	Input m_input;

	//---------------------------------------------------------
	// 表示確認用オブジェクト
	//---------------------------------------------------------
	std::unique_ptr<test>
		m_testSpriteObject;
};