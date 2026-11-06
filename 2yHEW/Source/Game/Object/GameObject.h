#pragma once
//コンポーネントを持つ側
#include "../Component/Base/ComponentBase.h"

#include <memory>
#include <vector>
#include <type_traits>

//AddComponent<T>()RemoveComponent<T>()GetComponent<T>()はテンプレート関数なので実装もヘッダーに書く
class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	//コンポーネント追加
	template<class T>
	std::shared_ptr<T> AddComponent()
	{
		//ComponentBaseを継承しているクラスのみ許可
		static_assert(std::is_base_of_v < ComponentBase, T>, "TはComponentBaesを継承する必要があります");

		//コンポーネント生成
		std::shared_ptr<T> component = std::make_shared<T>();

		//自分(GameObject)を持ち主として登録
		//Initializeより先に設定するのは、
		//Initialize野中で同じオブジェクトの別コンポーネントを探せるようにするため
		component->m_owner = this;

		//GameObjectが管理する
		m_components.push_back(component);

		//コンポーネント初期化
		component->Initialize();

		return component;
	}

	//コンポーネント削除
	template<class T>
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
				(*componentIterator)->m_owner = nullptr;

				componentIterator = m_components.erase(componentIterator);
				return;
			}
			++componentIterator;
		}
	}

	//コンポーネント取得
	template<class T>
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

	//付いている全コンポーネントを更新
	//コンポーネントが自分で動けるようにするため
	//舞フレームここからUpdate()を呼ぶ
	void UpdateComponents()
	{
		for (const auto& component : m_components)
		{
			component->Update();
		}
	}

protected:
	//登録されたコンポーネント
	std::vector<std::shared_ptr<ComponentBase>> m_components;
};

