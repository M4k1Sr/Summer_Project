#include "Panch.h"
#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/SphereCollider.h"

Panch::Panch(const Vector3& cactusPos)
	: ActorBase(),
	cactusPos(cactusPos)
{
}

void Panch::Load(void)
{
	AddCollider(new SphereCollider(COLLIDER_TAG::EnemyAttackBox,60.0f, trans.pos));
}

void Panch::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Player:

		SetJudgeFlg(false);

		break;
	default:
		break;
	}
}

void Panch::Start(void)
{
	SetJudgeFlg(true);

	//ç¿ïWê›íË
	trans.pos = cactusPos + CREATE_LOCAL_POS;
}


void Panch::SubUpdate(void)
{

}
