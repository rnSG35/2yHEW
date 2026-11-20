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
	/// <summary>
	/// コンストラクタ、使用する画像パスを受け取る
	/// </summary>
	/// <param name="filePath">使う画像ファイルのパス</param>
	SpriteObjectBase(const std::string& filePath);
	virtual ~SpriteObjectBase() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 描画する位置の設定
	/// </summary>
	/// <param name="x">横位置：右がプラス</param>
	/// <param name="y">縦位置：下がプラス</param>
	/// <param name="z">奥行き：無くてもいい、手前がプラス</param>
	void SetPosition(float x, float y,float z = 0.0f/*zは省略可能*/);

	/// <summary>
	/// 表示サイズ設定
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void SetSize(float width, float height);

	/// <summary>
	/// 色設定、基本的にしなくていい
	/// </summary>
	/// <param name="red">赤</param>
	/// <param name="green">緑</param>
	/// <param name="blue">青</param>
	/// <param name="alpha">透明度</param>
	void SetColor(
		float red,
		float green,
		float blue,
		float alpha);

	/// <summary>
	/// ブレンドステートの型、合成方法設定
	/// </summary>
	/// <param name="blendMode">ALPHA:通常の合成　ADDITIVE:光などに使う加算合成</param>
	void SetBlendMode(eSpriteBlendMode blendMode);

protected:

	virtual void OnInit() {};	//派生用
	virtual void OnUpdate() {};	//派生用
	virtual void OnDraw() {};	//派生用
	
private:
	//読み込んだTexture
	std::shared_ptr<Texture> m_texture;

	//string型、派生クラスのコンストラクタで設定する画像パス
	std::string m_filePath;

	//float型、画像の表示サイズ
	float m_width = 0.0f;
	float m_height = 0.0f;

	//XMFLOAT4描画するときに画像に掛ける色
	DirectX::XMFLOAT4 m_color =
	{
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};

	//eSpriteBlendMode、描画時の合成方法
	eSpriteBlendMode m_blendMode = eSpriteBlendMode::ALPHA;
};