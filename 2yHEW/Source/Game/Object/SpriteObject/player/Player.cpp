#include "Player.h"

Player::Player()
	: SpriteObjectBase(
		"AppData/Assets/Textures/laser.png")
{}

void Player::OnInit()
{
	// レーザーの位置
	SetPosition(Float2(500.0f,0.0f));

	// レーザーのサイズ
	SetSize(Float2(550.0f,500.0f));

	// 青白いレーザー
	SetColor(COLOR_WHITE);

	// 光なので加算合成
	SetBlendMode(eSpriteBlendMode::ADDITIVE);
}

void Player::OnUpdate()
{}

void Player::OnDraw()
{}