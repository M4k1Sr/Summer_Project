#include "DirtBlock.h"

#include "../../Common/Collider/BoxCollider.h"

DirtBlock::DirtBlock(const Vector3& pos) :
	StageBlockBase(STAGE_BLOCK_TAG::Dirt, pos,true, true)
{
	trans.pos = pos;
}

void DirtBlock::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/Dirt");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(100.0f, 73.242, 100.0f)));
}
