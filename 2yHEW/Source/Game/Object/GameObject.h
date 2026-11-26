#pragma once
//コンポーネントを持つ側
#include "../Component/Base/ComponentBase.h"

#include <memory>
#include <vector>
#include <concepts>
#include <utility>   // std::forward

class Input;
class SceneBase;

//AddComponent<T>()RemoveComponent<T>()GetComponent<T>()はテンプレート関数なので実装もヘッダーに書く
class GameObject
{
public:
	GameObject() = default;
	//派生クラスを削除するとき、virtualでないと派生クラス側のデストラクタが呼ばれないため
	virtual ~GameObject() = default;

	//基底クラスのポインタから呼んでも、派生クラス側の初期化が実行される
	virtual bool Initialize() { return true; }

	//何も上書きしない派生クラスでもコンポーネントだけは動くようにする(当たり判定などのため)
	virtual void Update() { UpdateComponents(); }

	//画像を持たないオブジェクトもあるので、基底では何もしない
	virtual void Draw() {}

	//終了
	virtual void UnInitialize() {}

	/// <summary>
	/// コンポーネント追加
	/// </summary>
	/// <typeparam name="...Args">追加したいコンポーネント</typeparam>
	/// <typeparam name="T">追加したいコンポーネント</typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<std::derived_from<ComponentBase> T, class...Args>
	std::shared_ptr<T> AddComponent(Args&&...args)
	{
		//コンポーネント生成
		//受け取った引数をそのままコンストラクタに渡す
		//std::forwardを使うと、Input&を参照のまま渡すことができる
		std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);


		//自分(GameObject)を持ち主として登録
		//Initializeより先に設定するのは、
		//Initializeの中で同じオブジェクトの別コンポーネントを探せるようにするため
		component->m_pOwner = this;

		//GameObjectが管理する
		m_components.push_back(component);

		//コンポーネント初期化
		component->Initialize();

		return component;
	}

	/// <summary>
	/// コンポーネント削除
	/// </summary>
	/// <typeparam name="T">削除したいコンポーネント</typeparam>
	template<std::derived_from<ComponentBase> T>
	void RemoveComponent()
	{
		for (auto componentIterator = m_components.begin();
			componentIterator != m_components.end();)
		{
			//指定された種類のコンポーネントか確認
			if (std::dynamic_pointer_cast<T>(*componentIterator))
			{
				//外す前に持ち主を解除
				//ほかの場所でshared_ptrが残っていた場合に
				//古い持ち主を触ってしまうのを防ぐため
				(*componentIterator)->m_pOwner = nullptr;

				componentIterator = m_components.erase(componentIterator);
				return;
			}
			++componentIterator;
		}
	}

	/// <summary>
	/// コンポーネント取得
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<std::derived_from<ComponentBase> T>
	std::shared_ptr<T> GetComponent()
	{
		for (const auto& component : m_components)
		{
			std::shared_ptr<T> findComponent =
				std::dynamic_pointer_cast<T>(component);

			if (findComponent)	return findComponent;
		}

		return nullptr;
	}

	/// <summary>
	/// コンポーネント更新
	/// </summary>
	void UpdateComponents()
	{
		for (const auto& component : m_components)
		{
			component->Update();
		}
	}

protected:
	//vector型、登録されたコンポーネントを保持する
	std::vector<std::shared_ptr<ComponentBase>> m_components;

	//入力、シーンが持っているものを指す
	//持ち主はGameManager
	Input* m_pInput = nullptr;

private:
	//m_pInputを設定できるのはSceneBaseだけ
	friend class SceneBase;

};