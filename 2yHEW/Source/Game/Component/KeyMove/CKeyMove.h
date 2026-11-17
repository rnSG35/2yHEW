#pragma once
#include "../Base/ComponentBase.h"

class Input;

class CKeyMove :
	public ComponentBase
{
public:
	CKeyMove(Input& input, float moveSpeed = 4.0f);
	~CKeyMove()  override = default;

	void Initialize() override;
	void Update() override;

private:
	//GameManagerが持つ1つのInputをすべてのシーンで共有しているので、コピーせず参照で持つ
	Input& m_input;

	//1フレームに動く料(ピクセル)
	float m_moveSpeed;

};