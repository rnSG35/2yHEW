#include "Player.h"

#include "../../../Component/Transform/CTransform.h"
#include "../../../../Framework/Input/input.h"
#include "../../../../Framework/Math/MathTypes.h"

#ifdef _DEBUG
#include "../../../../../thirdParty/imgui/include/imgui.h"
#endif

Player::Player()
	: SpriteObjectBase(
		"AppData/Assets/Textures/laser.png")
{}

void Player::OnInit()
{
	// レーザーの位置
	SetPosition(Float2(500.0f, 0.0f));

	// レーザーのサイズ
	SetSize(Float2(550.0f, 500.0f));

	// 青白いレーザー
	SetColor(COLOR_WHITE);

	// 光なので加算合成
	//SetBlendMode(eSpriteBlendMode::ADDITIVE);
	m_pPlayerTransform = GetComponent<CTransform>().get();
}


namespace
{
	//1回押したときに変わる前後関係(z)の量
	const float DEPTH_STEP = 1.0f;
}

void Player::OnUpdate()
{
	//入力かCTransformが設定されていなければ何もしない。nullptrを触って落ちないようにするため
	if (m_pInput == nullptr || m_pPlayerTransform == nullptr)
	{
		return;
	}

	//imguiの入力欄に文字を打っている間は、キー入力で移動しない
	//WASDで数値を打つと、動いてしまうのを防ぐため
	bool isTypingInImGui = false;
#ifdef _DEBUG
	isTypingInImGui = ImGui::GetIO().WantTextInput;
#endif
	if (!isTypingInImGui)
	{
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
		m_pPlayerTransform->Translate({ moveDirection.x, moveDirection.y, depthAmount });
	}

#ifdef _DEBUG	//デバッグ用画面
	ImGui::Begin("GameScene Debug");

	//座標を表示
	const Float3& currentPosition = m_pPlayerTransform->GetPosition();
	ImGui::Text("Player X:%.2f Y:%.2f Z:%.2f", currentPosition.x, currentPosition.y, currentPosition.z);

	//テキスト入力で座標を変える
	Float3 inputPosition = currentPosition;
	if (ImGui::InputFloat3("Player位置", &inputPosition.x, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_pPlayerTransform->SetPosition(inputPosition);
	}

	ImGui::End();
#endif

}

void Player::OnDraw()
{}

void Player::OnUnInit()
{
	m_pPlayerTransform = nullptr;
}