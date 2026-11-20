#pragma once
#include "../Base/ComponentBase.h"

#include "../../../../thirdParty/directxtk/include/SimpleMath.h"

//位置を管理するコンポーネント、UnityのTransform
class CTransform :
	public ComponentBase
{
public:
	/// <summary>
	/// Vector3型を使った位置の設定
	/// </summary>
	/// <param name="position">DirectXでのVector3型のposition</param>
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	/// <summary>
	/// 数値をそのまま使った位置の設定
	/// </summary>
	/// <param name="x">横位置</param>
	/// <param name="y">縦位置</param>
	/// <param name="z">奥行き</param>
	void SetPosition(float x, float y, float z) { m_position = DirectX::SimpleMath::Vector3(x, y, z); }

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>Vector3型のm_position</returns>
	const DirectX::SimpleMath::Vector3& GetPosition() const { return m_position; }

	/// <summary>
	/// 今の位置から、指定した量だけ動かす
	/// </summary>
	/// <param name="moveAmount">Vector3型の指定した位置からどれだけ動かすか</param>
	void Translate(const DirectX::SimpleMath::Vector3& moveAmount) { m_position += moveAmount; }

private:
	//Vector3型の位置を表す変数
	DirectX::SimpleMath::Vector3 m_position = DirectX::SimpleMath::Vector3::Zero;
};