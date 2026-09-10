#include "StageBlockBase.h"

#include "../../Common/Collider/BoxCollider.h"

StageBlockBase::StageBlockBase(STAGE_BLOCK_TAG BlockTag, const Vector3& pos, bool dynamicFlg,bool isGravity, bool isPushFlg) :
	ActorBase()
{
	trans.pos = pos;
	SetDynamicFlg(dynamicFlg);
	SetGravityFlg(isGravity);
	SetPushFlg(isPushFlg);
}