#pragma once
#include "../Base/ComponentBase.h"

#include "../../../../thirdParty/directxtk/include/SimpleMath.h"

//位置を管理するコンポーネント、UnityのTransform
class CTransform :
	public ComponentBase
{
public:
	//位置の設定
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	void SetPosition(float x, float y, float z) { m_position = DirectX::SimpleMath::Vector3(x, y, z); }

	//位置の取得
	const DirectX::SimpleMath::Vector3& GetPosition() const { return m_position; }

	//今の位置から、指定した量だけ動かす
	void Translate(const DirectX::SimpleMath::Vector3& moveAmount) { m_position += moveAmount; }

private:
	//位置
	DirectX::SimpleMath::Vector3 m_position = DirectX::SimpleMath::Vector3::Zero;
};