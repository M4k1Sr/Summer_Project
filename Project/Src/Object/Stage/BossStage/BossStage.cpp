#include "BossStage.h"

#include "../../../Utility/Utility.h"

#include "../../Common/Collider/BoxCollider.h"

BossStage::BossStage()
{
}

void BossStage::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(false);

	// 重力を有効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Stage/Stage");

	// モデルのスケール設定
	trans.scale.x = 1.5f;
	trans.scale.z = 1.5f;

#pragma endregion

#pragma region コライダーの生成

	// コライダー設定
	AddCollider(new BoxCollider(COLLIDER_TAG::Stage, Vector3(COLL_SIZE), Vector3::Yonly(COLL_POS_Y)));

#pragma endregion

}
