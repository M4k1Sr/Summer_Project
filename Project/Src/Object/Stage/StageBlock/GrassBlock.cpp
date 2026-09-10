#include "GrassBlock.h"

#include "../../Common/Collider/BoxCollider.h"

GrassBlock::GrassBlock(const Vector3& pos):
	StageBlockBase(STAGE_BLOCK_TAG::Grass, pos,true,true)
{
	trans.pos = pos;
}

void GrassBlock::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/Grass");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(100.0f, 73.242, 100.0f)));
}
