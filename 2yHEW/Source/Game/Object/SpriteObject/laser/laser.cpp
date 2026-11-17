#include "laser.h"

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
laser::laser()
	: SpriteObjectBase(
		"AppData/Assets/Textures/laser1.png")
{}

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
void laser::OnInit()
{
	// レーザーの位置
	SetPosition(
		100.0f,
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

void laser::OnUpdate()
{}

void laser::OnDraw()
{}