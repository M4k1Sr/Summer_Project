#include "BlockGrass.h"

#include "../../Common/Collider/BoxCollider.h"

BlockGrass::BlockGrass(const Vector3& pos,
	bool dynamicFlg,
	bool isGravity,
	bool isPushFlg
):
	StageBlockBase(STAGE_BLOCK_TAG::Grass, pos,dynamicFlg,isGravity,isPushFlg)
{
	trans.pos = pos;
}

void BlockGrass::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/Grass");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(200.0f, 150.0f, 600.0f)));
}
