#include "test.h"
#include "../../../Component/ShowObject/CShowObject.h"
#include "../../../../Framework/DebugSystem/DebugLog.h"

//---------------------------------------------------------
//コンストラクタ
//---------------------------------------------------------
test::test() : SpriteObjectBase("AppData/Assets/Textures/test.png")
{

}

void test::OnInit()
{
	//画面左上から見た表示位置
	SetPosition(100.0f, 100.0f);

	//画面に表示する大きさ
	SetSize(256.0f, 256.0f);

	auto showComponent =AddComponent<CShowObject>();

	if (showComponent->GetOwner() == this)
	{
		DebugLogWrite("CShowObjectのm_owner設定成功");
	}
}

void test::OnUpdate()
{
}

void test::OnDraw()
{
}
