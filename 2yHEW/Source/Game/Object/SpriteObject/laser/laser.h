#pragma once
#include "../SpriteObjectBase.h"
class laser :
	public SpriteObjectBase
{
public:
	laser();
	~laser() override = default;

private:
	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;
};