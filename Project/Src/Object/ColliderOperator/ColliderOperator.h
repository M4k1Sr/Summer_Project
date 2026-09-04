#pragma once
#include "../Common/Collider/AttackCollider.h"


class ColliderOperator
{
public:

	ColliderOperator(std::function<void(ColliderBase* newClass)> ColliderCreate);
	~ColliderOperator() = default;

	void Update(void);


	/// <summary>
	/// 攻撃判定の生成
	/// </summary>
	/// <param name="type"></param>
	/// <param name="count">攻撃判定の個数</param>
	/// <param name="duration">攻撃判定の持続時間</param>
	/// <param name="damage">ダメージ量(基本は１固定)</param>
	/// <param name="pos"></param>
	/// <returns></returns>
	AttackCollider* CreateAttackCollider(
		COLLIDER_TAG type, float radius, int count, float duration, int damage, const Vector3& pos);
	

private:


	std::function<void(ColliderBase* newClass)> ColliderCreate;

};


