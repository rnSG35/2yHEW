

#pragma once

#include <memory>
#include "../../Game/Object/SpriteObject/test/test.h"

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