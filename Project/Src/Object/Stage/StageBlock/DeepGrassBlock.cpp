#include "DeepGrassBlock.h"

#include "../../Common/Collider/BoxCollider.h"

DeepGrassBlock::DeepGrassBlock(const Vector3& pos,
	bool dynamicFlg, 
	bool isGravity, 
	bool isPushFlg) :
	StageBlockBase(STAGE_BLOCK_TAG::DeepGrass, pos, dynamicFlg, isGravity, isPushFlg)
{
	trans.pos = pos;
}

void DeepGrassBlock::Load(void)
{
	trans.LoadModel("Stage/StageMapChip/DeepGrass");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(200.0f, 150.0f, 1700.0f)));
}
