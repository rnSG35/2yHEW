#pragma once

#include <DirectXMath.h>
#include "../Math/MathTypes.h"

//2D用のカメラ
class Camera
{
public:
	/// <summary>
	/// 画面に映す範囲の大きさを設定する
	/// </summary>
	/// <param name="width">横幅(ピクセル)</param>
	/// <param name="height">縦幅(ピクセル)</param>
	void SetViewSize(float width, float height);

	/// <summary>
	/// カメラの位置を設定
	/// </summary>
	/// <param name="position">画面の中央に移すゲームの座標</param>
	void SetPosition(const Float2& position) { m_position = position; }

	/// <summary>
	/// カメラの位置を取得
	/// </summary>
	/// <returns>Float2型のm_position</returns>
	const Float2& GetPosition() const { return m_position; }

	/// <summary>
	/// ズームを設定
	/// </summary>
	/// <param name="zoom">1で等倍、2で2倍に拡大、0.5で半分に縮小</param>
	/// <remarks>0以下だと行列が壊れるので、小さな値に制限</remarks>
	void SetZoom(float zoom);

	/// <summary>
	/// ズームを取得
	/// </summary>
	/// <returns>float型のm_zoom</returns>
	float GetZoom() const { return m_zoom; }

	/// <summary>
	/// 目標の位置に少しずつ近づける
	/// </summary>
	/// <param name="targetPosition">追いかける位置(プレイヤーの位置など)</param>
	/// <param name="followRate">1フレームで近づく割合(0~1)、1ならすぐ追いつき、小さいほどゆっくり追う</param>
	/// <remarks>舞フレーム呼ぶ。すぐ追いつくより、少し遅れて王法が画面の揺れが少なく見やすいため
	void Follow(const Float2& targetPosition, float followRate);

	/// <summary>
	/// ゲームの座標を画面の座標に変換する行列を取得
	/// </summary>
	/// <returns>ビュー行列(カメラの位置)と正射影行列(移す範囲)をかけ合わせたもの</returns>
	DirectX::XMMATRIX GetViewProjectionMatrix() const;
private:
	Float2 m_position = FLOAT2_ZERO;

	float m_zoom = 1.0f;

	float m_viewWidth = 1280.0f;
	float m_viewHeight = 720.0f;

};