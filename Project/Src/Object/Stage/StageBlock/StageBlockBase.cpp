#include "StageBlockBase.h"

#include "../../Common/Collider/BoxCollider.h"

StageBlockBase::StageBlockBase(STAGE_BLOCK_TAG BlockTag, const Vector3& pos, bool dynamicFlg,bool isGravity, bool isPushFlg) :
	ActorBase()
{
	trans.pos = pos;
	SetDynamicFlg(dynamicFlg);	// 動的オブジェクトかどうかの設定
	SetGravityFlg(isGravity);	// 重力を適用するかどうかの設定
	SetPushFlg(isPushFlg);		// 衝突時押し出しを行うか否かの設定
}