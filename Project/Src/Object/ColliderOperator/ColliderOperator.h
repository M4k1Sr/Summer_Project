#pragma once
#include "../Common/Collider/AttackCollider.h"

class ColliderOperator
{
public:

	ColliderOperator(std::function<void(ColliderBase* newClass)> ColliderCreate);
	~ColliderOperator() = default;

	void Update(void);

	AttackCollider* CreateAttackCollider(
		COLLIDER_TAG type, float radius, int count, float duration, int damage, const Vector3& pos);
	

private:

	std::function<void(ColliderBase* newClass)> ColliderCreate;

};


