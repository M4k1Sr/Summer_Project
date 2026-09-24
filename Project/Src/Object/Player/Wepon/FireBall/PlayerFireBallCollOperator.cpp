#include "PlayerFireBallCollOperator.h"

#include "../../../Common/Collider/SphereCollider.h"

PlayerFireBallCollOperator::PlayerFireBallCollOperator(
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

void PlayerFireBallCollOperator::Load(void)
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


void PlayerFireBallCollOperator::SubUpdate(void)
{
	// 判定が発生しているときだけ処理する
	if (!attackColl->GetJudgeFlg()) { return; }

	// 前方に転がり続ける
	trans.pos += front * 10;

	// 寿命のカウントダウン(リトルフラワー)（計算方法は仮）
	lifeTimer -= 1.0f / 60.0f;
	if (lifeTimer <= 0.0f) {
		Off();
	}
}

void PlayerFireBallCollOperator::On(void)
{
	attackColl->SetJudgeFlg(true);

	// プレイヤーの座標、プレイヤーの角度、攻撃判定の相対座標をつかい
	// 判定の座標を割り出す
	trans.pos = playerTrans.pos + COLL_LOCAL_POS.TransMat(MGetRotY(playerTrans.angle.y));

	//進む方向を取得
	front = Vector3(sinf(playerTrans.angle.y),
		0.0f,
		cosf(playerTrans.angle.y)
	);

	// タイマーリセット
	lifeTimer = FIREBALL_LIFETIME_MAX;
}

void PlayerFireBallCollOperator::Off(void)
{
	attackColl->SetJudgeFlg(false);
}

void PlayerFireBallCollOperator::SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)
{
	velocity.y = (velocity.y < 0.0f) ? 5.0f : velocity.y;
}

void PlayerFireBallCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{

}

