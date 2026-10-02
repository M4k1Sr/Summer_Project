#include "BlockWall.h"

#include "../../Common/Collider/BoxCollider.h"

BlockWall::BlockWall(const Vector3& pos,
	bool dynamicFlg, 
	bool isGravity, 
	bool isPushFlg) :
	StageBlockBase(STAGE_BLOCK_TAG::Wall, pos, dynamicFlg, isGravity, isPushFlg)
{
	trans.pos = pos;
}

void BlockWall::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/WallBlock");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(200.0f, 150.0f, 200.0f)));
}
