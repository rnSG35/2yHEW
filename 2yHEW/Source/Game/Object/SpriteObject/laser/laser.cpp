#include "laser.h"

laser::laser()
	: SpriteObjectBase(
		"AppData/Assets/Textures/laser1.png")
{}

void laser::OnInit()
{
	// レーザーの位置
	SetPosition(Float2(100.0f,0.0f));

	// レーザーのサイズ
	SetSize(Float2(100.0f,100.0f));

	//色
	SetColor(COLOR_WHITE);

	// 光なので加算合成
	SetBlendMode(eSpriteBlendMode::ADDITIVE);
}

void laser::OnUpdate()
{}

void laser::OnDraw()
{}