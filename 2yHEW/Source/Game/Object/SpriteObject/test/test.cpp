#include "test.h"
#include "ShowObject/CShowObject.h"

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

	AddComponent<CShowObject>();
}

void test::OnUpdate()
{
}

void test::OnDraw()
{
}
