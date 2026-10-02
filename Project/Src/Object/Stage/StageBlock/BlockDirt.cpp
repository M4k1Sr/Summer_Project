#include "BlockDirt.h"

#include "../../Common/Collider/BoxCollider.h"

BlockDirt::BlockDirt(const Vector3& pos,
	bool dynamicFlg,
	bool isGravity,
	bool isPushFlg
) :
	StageBlockBase(STAGE_BLOCK_TAG::Dirt, pos,dynamicFlg,isGravity, isPushFlg)
{
	trans.pos = pos;
}

void BlockDirt::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/Dirt");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(200.0f, 150.0f, 200.0f)));
}
