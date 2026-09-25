#include "DirtBlock.h"

#include "../../Common/Collider/BoxCollider.h"

DirtBlock::DirtBlock(const Vector3& pos,
	bool dynamicFlg,
	bool isGravity,
	bool isPushFlg
) :
	StageBlockBase(STAGE_BLOCK_TAG::Dirt, pos,dynamicFlg,isGravity, isPushFlg)
{
	trans.pos = pos;
}

void DirtBlock::Load(void)
{
	// モデル読み込み
	trans.LoadModel("Stage/StageMapChip/Dirt");

	// コライダー生成
	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(200.0f, 150.0f, 200.0f)));
}
