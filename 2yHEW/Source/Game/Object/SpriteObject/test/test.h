#pragma once
#include "../SpriteObjectBase.h"

class CShowObject;

class Test :
	public SpriteObjectBase
{
public:
	Test();
	~Test() override = default;

private:
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
	void OnUnInit() override;

private:
	CShowObject* m_pShowComponent = nullptr;
};