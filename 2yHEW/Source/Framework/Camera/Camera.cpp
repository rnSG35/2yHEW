#include "Camera.h"

#include <algorithm>	//std::clamp std::max

using namespace DirectX;
namespace
{
	//ズームの最小値()0になると移す範囲が無限大になり、行列が壊れるため
	const float MIN_ZOOM = 0.01f;
}

//---------------------------------------------------------
//移す範囲の大きさを設定
//---------------------------------------------------------
void Camera::SetViewSize(float width, float height)
{
	m_viewWidth = width;
	m_viewHeight = height;
}

//---------------------------------------------------------
//ズーム設定
//---------------------------------------------------------
void Camera::SetZoom(float zoom)
{
	m_zoom = std::max(zoom, MIN_ZOOM);
}

//---------------------------------------------------------
//目標へ少しずつ近づける
//---------------------------------------------------------
void Camera::Follow(const Float2& targetPosition, float followRate)
{
	//割合を0~1の範囲にする、1以上にすると通り過ぎる
	const float rate = std::clamp(followRate, 0.0f, 1.0f);

	//「今の位置から目標までの差」の一部だけ進める
	m_position = m_position + (targetPosition - m_position) * rate;
}

DirectX::XMMATRIX Camera::GetViewProjectionMatrix() const
{
	//ビュー行列：カメラの位置が原点に来るように全体を逆向きに動かす
	const XMMATRIX viewMatrix = XMMatrixTranslation(-m_position.x, -m_position.y, 0.0f);

	//正射影行列：移す範囲を画面の座標(-1~1)に変換
	const XMMATRIX projectionMatrix = XMMatrixOrthographicLH(
		m_viewWidth / m_zoom,
		m_viewHeight / m_zoom,
		0.0f, 1.0f);

	//先にカメラの位置で動かし、次に画面の範囲へ変換
	return viewMatrix * projectionMatrix;
}
