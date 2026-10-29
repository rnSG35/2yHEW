#pragma once
//全コンポーネントの基底クラス
// GameObjectに付けられる機能の基底
//将来的にCTransform、CMove、CCollider、CAnimationなどをここから派生できるように
class ComponentBase
{
public:
	//デストラクタ
	virtual ~ComponentBase() = default;

	//初期化
	virtual void Initialize() {}

	//更新
	virtual void Update() {}
};

