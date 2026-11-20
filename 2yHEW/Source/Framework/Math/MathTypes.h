#pragma once

#include <DirectXMath.h>
#include <cmath>

//---------------------------------------------------------
// フレームワーク共通の数学の型
//
// DirectXMathの型(XMFLOAT2など)を継承しているので、
// DirectXの関数へそのまま渡せる
// XMFLOATには計算の機能がないため、使う分だけ追加する
//---------------------------------------------------------

//---------------------------------------------------------
// // Float2：x・y(移動方向・大きさなど)
//---------------------------------------------------------
struct Float2 : public DirectX::XMFLOAT2
{


	/// <summary>
	/// コンストラクタ
	/// XMFLOAT2の引数なしのコンストラクタは中身を初期化しないので自分で(0,0)にする
	/// 継承しているので{x,y}の形では作れない
	/// </summary>
	constexpr Float2() : XMFLOAT2(0.0f, 0.0f) {}
	constexpr Float2(float x, float y) : XMFLOAT2(x, y) {}

	/// <summary>
	/// DirectXの関数から受け取ったXMFLOAT2をFloat2にする
	/// </summary>
	/// <param name="v">XMFLOAT2型の変数</param>
	constexpr Float2(const DirectX::XMFLOAT2& v) : XMFLOAT2(v) {}

	Float2 operator+(const Float2& v) const { return { x + v.x, y + v.y }; }
	Float2 operator-(const Float2& v) const { return { x - v.x, y - v.y }; }
	Float2 operator*(float s) const { return { x * s, y * s }; }
	Float2& operator+=(const Float2& v) { x += v.x; y += v.y; return *this; }

	/// <summary>
	/// 長さを取得する
	/// </summary>
	float Length() const { return std::sqrt(x * x + y * y); }

	/// <summary>
	/// 向きはそのままで、長さを1にしたものを返す
	/// </summary>
	/// <returns>長さが0の場合は(0, 0)</returns>
	/// <remarks>0で割ると値が壊れる(NaNになる)ため、長さ0のときは計算しない</remarks>
	Float2 Normalized() const
	{
		float length = Length();
		return (length > 0.0f) ? Float2(x / length, y / length) : Float2();
	}
};

//---------------------------------------------------------
// Float3：x・y・z(位置など)
//
// 2Dゲームでは x:左右 y:上下 z:前後関係(描画順) として使う
//---------------------------------------------------------
struct Float3 : public DirectX::XMFLOAT3
{
	constexpr Float3() : XMFLOAT3(0.0f, 0.0f, 0.0f) {}
	constexpr Float3(float x, float y, float z) : XMFLOAT3(x, y, z) {}
	constexpr Float3(const DirectX::XMFLOAT3& v) : XMFLOAT3(v) {}

	Float3 operator+(const Float3& v) const { return { x + v.x, y + v.y, z + v.z }; }
	Float3 operator-(const Float3& v) const { return { x - v.x, y - v.y, z - v.z }; }
	Float3 operator*(float s) const { return { x * s, y * s, z * s }; }
	Float3& operator+=(const Float3& v) { x += v.x; y += v.y; z += v.z; return *this; }
};

//---------------------------------------------------------
// // Float4：x・y・z・w(色など)
// 色として使う場合は x:赤 y:緑 z:青 w:不透明度(0〜1)
//---------------------------------------------------------
struct Float4 : public DirectX::XMFLOAT4
{
	constexpr Float4() : XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {}
	constexpr Float4(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
	constexpr Float4(const DirectX::XMFLOAT4& v) : XMFLOAT4(v) {}
};

//---------------------------------------------------------
// よく使う値(定数なのですべて大文字)
//---------------------------------------------------------
inline constexpr Float2 FLOAT2_ZERO(0.0f, 0.0f);
inline constexpr Float3 FLOAT3_ZERO(0.0f, 0.0f, 0.0f);
inline constexpr Float4 COLOR_WHITE(1.0f, 1.0f, 1.0f, 1.0f);