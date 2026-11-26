#include "Player.h"

#include "../../../Component/Transform/CTransform.h"
#include "../../../../Framework/Input/input.h"
#include "../../../../Framework/Math/MathTypes.h"
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


namespace
{
	//1回押したときに変わる前後関係(z)の量
	const float DEPTH_STEP = 1.0f;
}

void Player::OnUpdate()
{
	//入力が設定されていなければ何もしない。シーンを通さずに作られた場合に、nullptrを触って落ちないようにするため
	if (m_pInput == nullptr)
	{
		return;
	}

	//自身のCTransformを取得
	std::shared_ptr<CTransform> transform = GetComponent<CTransform>();

	if (!transform)
	{
		return;
	}

	//上下左右を押している間、移動する
	Float2 moveDirection = FLOAT2_ZERO;
	if (m_pInput->GetKeyPress(VK_LEFT) || m_pInput->GetKeyPress(VK_A)) { moveDirection.x -= 1.0f; }
	if (m_pInput->GetKeyPress(VK_RIGHT) || m_pInput->GetKeyPress(VK_D)) { moveDirection.x += 1.0f; }
	if (m_pInput->GetKeyPress(VK_UP) || m_pInput->GetKeyPress(VK_W)) { moveDirection.y += 1.0f; }
	if (m_pInput->GetKeyPress(VK_DOWN) || m_pInput->GetKeyPress(VK_S)) { moveDirection.y -= 1.0f; }

	moveDirection = moveDirection.Normalized() * m_moveSpeed;
	//前後関係(押した瞬間に1段ずつ変える)
	float depthAmount = 0.0f;

	if (m_pInput->GetKeyTrigger('Q')) { depthAmount -= DEPTH_STEP; }   //手前へ
	if (m_pInput->GetKeyTrigger('E')) { depthAmount += DEPTH_STEP; }   //奥へ

	//2Dの移動量と前後関係の変化をまとめて動かす
	transform->Translate({ moveDirection.x, moveDirection.y, depthAmount });
}

void Player::OnDraw()
{}