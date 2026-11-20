#pragma once
//全コンポーネントの基底クラス
// GameObjectに付けられる機能の基底
//将来的にCTransform、CMove、CCollider、CAnimationなどをここから派生できるように

//GameObject.hをインクルードすると
//GameObject.h→ComponentBase.h→GameObject.h
//という循環インクルードになるため、名前だけ伝える
//ポインタとして持つなら中身を知らなくても問題ない
class GameObject;

class ComponentBase
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ComponentBase() = default;

	/// <summary>
	/// 初期化、AddComponent()関数の中でm_ownerを設定した後に呼ばれるため
	/// ここではm_ownerを使用可能
	/// </summary>
	virtual void Initialize() {}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// 自分が付いているGameObjectを取得
	/// </summary>
	/// <returns>自分がついているGameObjectのポインタ</returns>
	GameObject* GetOwner() const { return m_pOwner; }

protected:
	//自分が付いているGameObject
	//所有しているのはGameObject側なので、解放の責任を持たない普通のポインタ
	//shared_ptrにすると、GameobjectとComponentがお互いを持ち合って解放されなくなるため使わない
	GameObject* m_pOwner = nullptr;

private:
	//m_ownerを設定できるのはGameObjectのみ
	//コンポーネント地震や外部から付け替えができると
	//どこに付いているのかが分からなくなるため
	friend class GameObject;
};