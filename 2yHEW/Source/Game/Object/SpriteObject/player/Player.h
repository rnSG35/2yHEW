#pragma once
#include "../SpriteObjectBase.h"

class CTransform;

class Player :
	public SpriteObjectBase
{
public:
	Player();
	~Player() override = default;

private:
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;

	//1フレームの動く量(ピクセル)
	float m_moveSpeed = 4.0f;

	CTransform* m_pPlayerTransform = nullptr;
};