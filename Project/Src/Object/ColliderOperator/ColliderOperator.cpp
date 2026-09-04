#include "ColliderOperator.h"
#include "../Common/ActorBase/ActorBase.h"

ColliderOperator::ColliderOperator(std::function<void(ColliderBase* newClass)> ColliderCreate)
	: ColliderCreate(ColliderCreate)
{
}

AttackCollider* ColliderOperator::CreateAttackCollider(COLLIDER_TAG type, float radius, int count, float duration, int damage,
    const Vector3& pos)
{
    AttackCollider* attackCollider =
        new AttackCollider(
            type,
            radius,
            pos
        );

    // 所持者ののコライダーリストへ追加
	ColliderCreate(attackCollider);


    return attackCollider;
}
