#include "PlayerWaterCollOperator.h"
#include <cstdlib>

#include "../../../Common/Collider/SphereCollider.h"

PlayerWaterCollOperator::PlayerWaterCollOperator(
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
	ColliderCreate(attackColl);
}


void PlayerWaterCollOperator::SubUpdate(void)
{
	if (!attackColl->GetJudgeFlg()) { return; }

	// 重力による落下の影響を加える
	velocity.y -= 0.2f;

	// 移動処理
	trans.pos += velocity;

	// 寿命処理
	lifeTimer -= 1.0f / 60.0f;
	if (lifeTimer <= 0.0f) {
		Off();
	}
}

void PlayerWaterCollOperator::On(void)
{
	attackColl->SetJudgeFlg(true);

	// プレイヤーの座標と回転から初期位置を設定
	trans.pos = playerTrans.pos + COLL_LOCAL_POS.TransMat(MGetRotY(playerTrans.angle.y));

	// 1. 基準となる前方方向ベクトル
	Vector3 baseFront = Vector3(
		sinf(playerTrans.angle.y),
		0.0f,
		cosf(playerTrans.angle.y)
	);

	// 2. 放水用の拡散（ランダムなブレ）を加える
	float spreadX = ((float)rand() / RAND_MAX - 0.5f) * 0.2f;
	float spreadY = ((float)rand() / RAND_MAX - 0.2f) * 0.2f;

	Vector3 dir = (baseFront + Vector3(spreadX, spreadY, 0.0f)).Normalized();

	// 3. 飛ばすスピードを設定
	float speed = 8.0f + ((float)rand() / RAND_MAX) * 4.0f;

	velocity = dir * speed;

	// タイマーリセット
	lifeTimer = FIREBALL_LIFETIME_MAX;
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

}

