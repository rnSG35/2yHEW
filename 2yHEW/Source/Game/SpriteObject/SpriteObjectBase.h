#pragma once
#include <memory>
#include <string>

class Texture;

class SpriteObjectBase
{
public:
	SpriteObjectBase() = default;
	virtual ~SpriteObjectBase() = default;

	//‰Šú‰»
	void Init();

	//XV
	void Update();

	//•`‰æ
	void Draw();

protected:
	//”h¶—p
	virtual void OnInit() {};
	virtual void OnUpdate();
	virtual void OnDraw() {};

protected:
	std::shared_ptr<Texture> m_texture;
	
	std::string m_filePath;
};

