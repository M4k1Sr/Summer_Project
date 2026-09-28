#include "PlayerThunderCollOperator.h"
#include <cstdlib>

#include "../../../Common/Collider/SphereCollider.h"

PlayerThunderCollOperator::PlayerThunderCollOperator(
	float COLL_RADIUS,
	const Vector3& COLL_LOCAL_POS,

	const Transform& playerTrans
) :
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans),

	attackColl(new SphereCollider(COLLIDER_TAG::Fire, COLL_RADIUS)),
	lifeTimer(0.0f)
{
}

void PlayerThunderCollOperator::Load(void)
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
	ColliderCreate(attackColl);
}


void PlayerThunderCollOperator::SubUpdate(void)
{
	if (!attackColl->GetJudgeFlg()) { return; }
	// 寿命処理
	lifeTimer -= 1.0f / 60.0f;
	if (lifeTimer <= 0.0f) {
		Off();
	}
}

void PlayerThunderCollOperator::On(const Vector3& offSet)
{
	attackColl->SetJudgeFlg(true);

	// プレイヤーの座標と回転から初期位置を設定
	trans.pos = playerTrans.pos + COLL_LOCAL_POS.TransMat(MGetRotY(playerTrans.angle.y));

	Vector3 baseFront = Vector3(
		sinf(playerTrans.angle.y),
		0.0f,
		cosf(playerTrans.angle.y)
	);


	trans.pos += (baseFront * offSet.z) + Vector3(0.0f, offSet.y, 0.0f);

	// タイマーリセット
	lifeTimer = THUNDER_LIFETIME_MAX;
}

void PlayerThunderCollOperator::Off(void)
{
	attackColl->SetJudgeFlg(false);
}

void PlayerThunderCollOperator::SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	velocity.y = (velocity.y < 0.0f) ? 0.0f : velocity.y;
	// 寿命処理
	Off();
}

void PlayerThunderCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{

}

