#include "BlockGrass.h"

#include "../../Common/Collider/BoxCollider.h"

BlockGrass::BlockGrass(const Vector3& pos,
	bool dynamicFlg,
	bool isGravity,
	bool isPushFlg
) :
	StageBlockBase(STAGE_BLOCK_TAG::Grass, pos, dynamicFlg, isGravity, isPushFlg)
{
	trans.pos = pos;
}

void BlockGrass::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/Grass");

	AddCollider(new BoxCollider(COLLIDER_TAG::Stage, COLL_SIZE));
}
