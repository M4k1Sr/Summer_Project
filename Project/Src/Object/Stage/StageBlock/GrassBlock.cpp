#include "GrassBlock.h"

#include "../../Common/Collider/BoxCollider.h"

GrassBlock::GrassBlock(const Vector3& pos, 
	bool dynamicFlg,
	bool isGravity,
	bool isPushFlg
):
	StageBlockBase(STAGE_BLOCK_TAG::Grass, pos,dynamicFlg,isGravity,isPushFlg)
{
	trans.pos = pos;
}

void GrassBlock::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/GrassStage");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(200.0f, 150.0f, 200.0f)));
}
