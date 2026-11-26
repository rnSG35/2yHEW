#include "test.h"
#include "../../../Component/ShowObject/CShowObject.h"
#include "../../../Component/Transform/CTransform.h"

#include "../../../../Framework/Input/input.h"
#include "../../../../Framework/DebugSystem/DebugLog.h"

#ifdef _DEBUG
#include "../../../../../thirdParty/imgui/include/imgui.h"
#endif
Test::Test()
	: SpriteObjectBase(
		"AppData/Assets/Textures/test.png")
{}

void Test::OnInit()
{
	//画面左上から見た表示位置
	SetPosition(Float2(100.0f, 100.0f));

	//画面に表示する大きさ
	SetSize(Float2(256.0f, 256.0f));

	m_pShowComponent = GetComponent<CShowObject>().get();
	m_pShowComponent->Hide();

	if (m_pShowComponent->GetOwner() == this)
	{
		//DebugLogWrite("CShowObjectのm_owner設定成功");
	}
}

void Test::OnUpdate()
{

	if (m_pInput && m_pInput->GetKeyTrigger(VK_SPACE))
	{
		if (m_pShowComponent)
		{
			m_pShowComponent->Toggle();
		}
	}

	if (m_pInput && m_pInput->GetKeyPress(VK_R))
	{
		std::shared_ptr<CTransform> transform = GetComponent<CTransform>();
		if (transform)
		{
			transform->Rotate(Float3(0.0f, 0.0f, 2.0f));
		}
	}

#ifdef _DEBUG	//デバッグ用画面
	ImGui::Begin("GameScene Debug");

	//フレームレートを表示
	ImGui::Text("FPS:%.2f", ImGui::GetIO().Framerate);

	if (m_pShowComponent)
	{
		//一度コピー
		bool isVisible = m_pShowComponent->IsVisible();

		if (ImGui::Checkbox("test表示オンオフ", &isVisible))
		{
			m_pShowComponent->SetVisible(isVisible);
		}
	}

	ImGui::End();
#endif

}

void Test::OnDraw()
{

}

void Test::OnUnInit()
{
	m_pShowComponent = nullptr;
}