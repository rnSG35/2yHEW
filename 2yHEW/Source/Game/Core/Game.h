

#pragma once

#include "../../Framework/Input/Input.h"

#include <memory>
#include "../Object/SpriteObject/test/test.h"
#include "../Object/SpriteObject/laser/laser.h"

class test;
class laser;

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

	// 表示確認用オブジェクト
	std::unique_ptr<test>
		m_testSpriteObject;
	std::unique_ptr<laser>
		m_laserSpriteObject;
};