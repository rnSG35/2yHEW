#include "CKeyMove.h"
#include "../Transform/CTransform.h"
#include "../../Object/GameObject.h"
#include "../../../Framework/Input/input.h"
#include "../../../Framework/DebugSystem/DebugLog.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

namespace
{
	//1回押したときに変わる前後関係(z)の量
	const float DEPTH_STEP = 1.0f;
}

//---------------------------------------------------------
//コンストラクタ
//---------------------------------------------------------
CKeyMove::CKeyMove(Input& input, float moveSpeed) : m_input(input), m_moveSpeed(moveSpeed)
{}

//---------------------------------------------------------
//初期化
//---------------------------------------------------------
void CKeyMove::Initialize()
{
	//CTransformが付いていないと動かせない
	if (m_pOwner && !m_pOwner->GetComponent<CTransform>())
	{
		DebugLogWrite("CKeyMoveを付けたオブジェクトにCTransformがない / CTransform not found");
	}
}

//---------------------------------------------------------
//更新
//---------------------------------------------------------
void CKeyMove::Update()
{
	if (m_pOwner == nullptr)
	{
		return;
	}

	//同じGameObjectに付いているCTransformを探す
	//初期化関数で保存せずに毎回探すのは、途中でCTransformが外された場合に、消えたものを触らないようにするため
	std::shared_ptr<CTransform> transform = m_pOwner->GetComponent<CTransform>();

	if (!transform)
	{
		return;
	}

	//上下左右を押している間、移動する
	Vector2 moveDirection = Vector2::Zero;
	if (m_input.GetKeyPress(VK_LEFT) || m_input.GetKeyPress(VK_A)) { moveDirection.x -= 1.0f; }
	if (m_input.GetKeyPress(VK_RIGHT)|| m_input.GetKeyPress(VK_D)) { moveDirection.x += 1.0f; }
	if (m_input.GetKeyPress(VK_UP)	 || m_input.GetKeyPress(VK_W)) { moveDirection.y -= 1.0f; }
	if (m_input.GetKeyPress(VK_DOWN) || m_input.GetKeyPress(VK_S)) { moveDirection.y += 1.0f; }
	//斜め移動
	if (moveDirection.LengthSquared() > 0.0f)
	{
		moveDirection.Normalize();
	}


	// 前後関係(押した瞬間に1段ずつ変える)
	//
	// zは描画順に使うだけなので、押している間ずっと変わるより
	// 1回ずつの方が確認しやすい
	float depthAmount = 0.0f;

	if (m_input.GetKeyTrigger('Q')) { depthAmount -= DEPTH_STEP; }   // 手前へ
	if (m_input.GetKeyTrigger('E')) { depthAmount += DEPTH_STEP; }   // 奥へ

	transform->Translate(Vector3(
		moveDirection.x * m_moveSpeed,
		moveDirection.y * m_moveSpeed,
		depthAmount));
}