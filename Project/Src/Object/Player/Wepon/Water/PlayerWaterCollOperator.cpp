#include "PlayerWaterCollOperator.h"
#include <cstdlib>

#include "../../../Common/Collider/SphereCollider.h"

#include "../../../../Utility/Utility.h"

#include "../../../../Manager/TimeScale/TimeScale.h"

PlayerWaterCollOperator::PlayerWaterCollOperator(
	float COLL_RADIUS,
	const Vector3& COLL_LOCAL_POS,

	const Transform& playerTrans
) :
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans),

	attackColl(new SphereCollider(COLLIDER_TAG::Water, COLL_RADIUS)),
	lifeTimer(0.0f)
{
}

void PlayerWaterCollOperator::Load(void)
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


void PlayerWaterCollOperator::SubUpdate(void)
{
	if (!attackColl->GetJudgeFlg()) { return; }

	waterVec.y -= 0.5f;

	// 移動処理
	trans.pos += waterVec;

	// 寿命処理
	lifeTimer -= DELTA_TIME * TimeScale::Get();
	if (lifeTimer <= 0.0f) {
		Off();
	}
}

void PlayerWaterCollOperator::On(void)
{
	attackColl->SetJudgeFlg(true);

	// プレイヤーの座標と回転から初期位置を設定
	trans.pos = playerTrans.pos + playerTrans.rotation.Rotate(COLL_LOCAL_POS);

	Vector3 baseFront = playerTrans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));

	Vector3 launchDir = baseFront;
	launchDir.y = 2.0f;

	// 放水用の拡散
	float spreadX = ((float)rand() / RAND_MAX - 0.5f) * 0.15f;
	float spreadY = ((float)rand() / RAND_MAX - 0.5f) * 0.10f;

	Vector3 dir = (launchDir + Vector3(spreadX, spreadY, 0.0f)).Normalized();

	float speed = 12.0f + ((float)rand() / RAND_MAX) * 3.0f;

	waterVec = dir * speed;

	// タイマーリセット
	lifeTimer = WATER_LIFETIME_MAX;
}

void PlayerWaterCollOperator::Off(void)
{
	attackColl->SetJudgeFlg(false);
}

void PlayerWaterCollOperator::SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	velocity.y = (velocity.y < 0.0f) ? 0.0f : velocity.y;
}

void PlayerWaterCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	//Off();
	other.GetActor()->TakeDamage(1);
}

