#include "Player.h"

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
Player::Player()
	: SpriteObjectBase(
		"AppData/Assets/Textures/laser.png")
{}

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
void Player::OnInit()
{
	// レーザーの位置
	SetPosition(
		550.0f,
		0.0f);

	// レーザーのサイズ
	SetSize(
		600.0f,
		600.0f);

	// 青白いレーザー
	SetColor(
		1.0f,
		1.0f,
		1.0f,
		1.0f);

	// 光なので加算合成
	SetBlendMode(eSpriteBlendMode::ADDITIVE);
}

void Player::OnUpdate()
{}

void Player::OnDraw()
{}