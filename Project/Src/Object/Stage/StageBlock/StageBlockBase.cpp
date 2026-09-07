#include "StageBlockBase.h"

#include "../../Common/Collider/BoxCollider.h"

StageBlockBase::StageBlockBase(STAGE_BLOCK_TAG BlockTag, const Vector3& pos, bool dynamicFlg, bool isPushFlg) :
	ActorBase()
{
	trans.pos = pos;
	BlockTag = BlockTag;
}