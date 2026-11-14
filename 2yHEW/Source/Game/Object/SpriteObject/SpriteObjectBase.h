#pragma once

#include "../GameObject.h"

#include <memory>
#include <string>
#include "../../../Framework/Renderer/SpriteRenderer.h"

class Texture;

class SpriteObjectBase :
	public GameObject
{
public:
	//使用する画像パスを受け取る
	SpriteObjectBase(const std::string& filePath);
	virtual ~SpriteObjectBase() = default;

	bool Initialize() override;
	void Update() override;
	void Draw() override;

	//表示位置設定
	void SetPosition(float x, float y);

	//表示サイズ設定
	void SetSize(float width, float height);

	void SetColor(
		float red,
		float green,
		float blue,
		float alpha);

	void SetBlendMode(eSpriteBlendMode blendMode);

protected:
	//派生用
	virtual void OnInit() {};
	virtual void OnUpdate() {};
	virtual void OnDraw() {};
	
private:
	//読み込んだTexture
	std::shared_ptr<Texture> m_texture;

	//派生クラスのコンストラクタで設定する画像パス
	std::string m_filePath;

	//画像左上の表示位置
	float m_x = 0.0f;
	float m_y = 0.0f;

	//画像の表示サイズ
	float m_width = 0.0f;
	float m_height = 0.0f;

	//描画するときに画像に掛ける色
	DirectX::XMFLOAT4 m_color =
	{
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};

	//描画時の合成方法
	eSpriteBlendMode m_blendMode = eSpriteBlendMode::ALPHA;
};