#include "test.h"
#include "../../../Component/ShowObject/CShowObject.h"
#include "../../../../Framework/DebugSystem/DebugLog.h"

test::test()
	: SpriteObjectBase(
		"AppData/Assets/Textures/test1.png")
{}

void test::OnInit()
{
	//画面左上から見た表示位置
	SetPosition(Float2(100.0f,100.0f));

	//画面に表示する大きさ
	SetSize(Float2(256.0f,256.0f));

	auto showComponent = AddComponent<CShowObject>();

	if (showComponent->GetOwner() == this)
	{
		//DebugLogWrite("CShowObjectのm_owner設定成功");
	}
}

void test::OnUpdate()
{}

void test::OnDraw()
{}