#pragma once

#include "../GameObject.h"

#include <memory>
#include <string>
#include "../../../Framework/Renderer/SpriteRenderer.h"
#include "../../../Framework/Math/MathTypes.h"

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
	/// 表示位置設定(Float3で指定)
	/// </summary>
	/// <param name="position">x:横位置　y:縦位置　z:前後</param>
	void SetPosition(const Float3& position);
	/// <summary>
	/// 描画する位置の設定(Float2とz数値で指定)
	/// </summary>
	/// <param name="position">x:横位置　y:縦位置</param>
	/// <param name="z">前後：(省略で0)</param>
	void SetPosition(const Float2& position, float z = 0.0f);
	/// <summary>
	/// 表示位置設定(数値で指定)
	/// </summary>
	/// <param name="x">横位置：右がプラス</param>
	/// <param name="y">縦位置：上がプラス</param>
	/// <param name="z">前後：奥がプラス・位置は画像の中心</param>
	void SetPosition(float x, float y, float z = 0.0f/*zは省略可能*/);



	/// <summary>
	/// 表示サイズ設定(Float2で指定)
	/// </summary>
	/// <param name="size">x:横幅　y:縦幅</param>
	void SetSize(const Float2& size);
	/// <summary>
	/// 数値で指定
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void SetSize(float width, float height);



	/// <summary>
	/// 色設定(数値で指定)、基本的にしなくていい
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
	/// 色設定(Float4)
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Float4& color);

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

	//Float2型、画像の表示サイズ
	Float2 m_size = FLOAT2_ZERO;

	//Float4型、描画するときに画像に掛ける色
	Float4 m_color = COLOR_WHITE;

	//eSpriteBlendMode、描画時の合成方法
	eSpriteBlendMode m_blendMode = eSpriteBlendMode::ALPHA;
};