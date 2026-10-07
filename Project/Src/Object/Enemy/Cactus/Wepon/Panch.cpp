#include "Panch.h"
#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/SphereCollider.h"

Panch::Panch(const Vector3 cactusPos)
{
}

void Panch::Load(void)
{
}

void Panch::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Player:
		state = STATE::None;
		
		SetJudge(false);

		break;
	case COLLIDER_TAG::Enemy:
		break;
	case COLLIDER_TAG::Stage:
		break;
	default:
		break;
	}
}

void Panch::Start(void)
{
	SetJudge(true);
}

void Panch::SubUpdate(void)
{
}
