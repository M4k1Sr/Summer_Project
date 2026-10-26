#include "PlayerFrostBlade.h"

#include "../../../Common/Collider/CapsuleCollider.h"

PlayerFrostBlade::PlayerFrostBlade(
	float COLL_RADIUS,
	const Vector3& COLL_LOCAL_POS,
	const Vector3& COLL_START_POS,
	const Vector3& COLL_END_POS,
	const Transform& playerTrans
) :
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans),

	attackColl(new CapsuleCollider(COLLIDER_TAG::FrostBlade, COLL_START_POS, COLL_END_POS, COLL_RADIUS))

{
}

void PlayerFrostBlade::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力をにする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

#pragma endregion

	attackColl->SetJudgeFlg(false);

	// コライダーを追加
	AddCollider(attackColl);
}


void PlayerFrostBlade::SubUpdate(void)
{
	// 判定が発生しているときだけ処理する
	if (!attackColl->GetJudgeFlg()) { return; }

}

void PlayerFrostBlade::On(void)
{
	attackColl->SetJudgeFlg(true);

	// プレイヤーの座標、プレイヤーの角度、攻撃判定の相対座標をつかい
	// 判定の座標を割り出す
	trans.pos = playerTrans.pos + playerTrans.rotation.Rotate(COLL_LOCAL_POS);

	//進む方向を取得
	front = playerTrans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
}

void PlayerFrostBlade::Off(void)
{
	attackColl->SetJudgeFlg(false);
}

void PlayerFrostBlade::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{

}

