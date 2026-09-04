#include "GrassStageBlock.h"

#include "../../Common/Collider/BoxCollider.h"

GrassStageBlock::GrassStageBlock(const Vector3& pos):
	ActorBase()
{
	trans.pos = pos;
}

void GrassStageBlock::Load(void)
{
	SetDynamicFlg(false);
	SetPushFlg(true);

	trans.LoadModel("Stage/StageMapChip/Grass");

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(100.0f, 73.242, 100.0f)));
}
