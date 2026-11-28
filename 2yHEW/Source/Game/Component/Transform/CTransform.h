#pragma once
#include "../Base/ComponentBase.h"

//#include "../../../Framework/Math/MathTypes.h"
//#include "../../../../thirdParty/directxtk/include/SimpleMath.h"

//位置を管理するコンポーネント、UnityのTransform
class CTransform :
	public ComponentBase
{
public:
	/// <summary>
	/// Float3を使った位置の設定
	/// </summary>
	/// <param name="position">x:(右がプラス) y:(上がプラス) z:(小さいほど手前)</param>
	void SetPosition(const Float3& position) { m_position = position; }
	/// <summary>
	/// 数値をそのまま使った位置の設定
	/// </summary>
	/// <param name="x">横位置</param>
	/// <param name="y">縦位置</param>
	/// <param name="z">奥行き</param>
	void SetPosition(float x, float y, float z) { m_position = Float3(x, y, z); }
	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>Float3型のm_position</returns>
	const Float3& GetPosition() const { return m_position; }
	/// <summary>
	/// 今の位置から、指定した量だけ動かす
	/// </summary>
	/// <param name="moveAmount">Float3型の指定した位置からどれだけ動かすか</param>
	void Translate(const Float3& moveAmount) { m_position += moveAmount; }



	/// <summary>
	/// Float3を使って回転の指定
	/// </summary>
	/// <param name="rotation">単位は度。z:画面上の回転(プラスで反時計回り)。x,yは未使用</param>
	void SetRotation(const Float3& rotation) { m_rotation = rotation; }
	/// <summary>
	/// 数値をそのまま使った回転を設定
	/// </summary>
	/// <param name="x">未使用(0)</param>
	/// <param name="y">未使用(0)</param>
	/// <param name="z">画面上の回転(度、プラスで時計回り)</param>
	void SetRotation(float x, float y, float z) { m_rotation = Float3(x, y, z); }
	/// <summary>
	/// 回転を取得
	/// </summary>
	/// <returns>現在の回転(度)</returns>
	const Float3& GetRotation() const { return m_rotation; }
	/// <summary>
	/// 今の回転から、指定した量だけ回す
	/// </summary>
	/// <param name="rotateAmount">回す量(度)。z:画面上の回転</param>
	void Rotate(const Float3& rotateAmount) { m_rotation += rotateAmount; }
private:
	//Float3型の位置を表す変数
	Float3 m_position = FLOAT3_ZERO;

	//Float3型の回転を表す変数
	Float3 m_rotation = FLOAT3_ZERO;
};