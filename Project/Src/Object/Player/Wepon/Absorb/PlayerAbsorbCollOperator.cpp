#include "PlayerAbsorbCollOperator.h"

#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../../Manager/Camera/CurrentCamera.h"

PlayerAbsorbCollOperator::PlayerAbsorbCollOperator(
	std::function<void(const COLLIDER_TAG& tag)> setAbility,
	std::function<void(const float angle)> setAngle,
	float COLL_RADIUS,

	const Vector3& COLL_LOCAL_POS,
	const Vector3& COLL_START_POS,
	const Vector3& COLL_END_POS,

	const Transform& playerTrans
) :

	setAbility(setAbility),
	setAngle(setAngle),

	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans),

	attackColl(new CapsuleCollider(COLLIDER_TAG::Absorb, COLL_START_POS, COLL_END_POS, COLL_RADIUS))
{
}

void PlayerAbsorbCollOperator::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	attackColl->SetJudgeFlg(false);

	// コライダーを追加
	ColliderCreate(attackColl);
}


void PlayerAbsorbCollOperator::SubUpdate(void)
{
	// 判定が発生しているときだけ処理する
	if (!attackColl->GetJudgeFlg()) { return; }

	// 追従し続ける
	trans.pos = playerTrans.pos + COLL_LOCAL_POS.TransMat(MGetRotY(playerTrans.angle.y));
}

void PlayerAbsorbCollOperator::On(void)
{
	attackColl->SetJudgeFlg(true);

	// プレイヤーの座標、プレイヤーの角度、攻撃判定の相対座標をつかい
	// 判定の座標を割り出す
	trans.pos = playerTrans.pos + COLL_LOCAL_POS.TransMat(MGetRotY(playerTrans.angle.y));

	isAbsorbed = false;
}

void PlayerAbsorbCollOperator::Off(void)
{
	attackColl->SetJudgeFlg(false);

	isAbsorbed = false;
}

void PlayerAbsorbCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Icicle:
		if (isAbsorbed) { break; }
		isAbsorbed = true;
		setAbility(COLLIDER_TAG::Water);
		setAngle(FaceCamera());
		break;
	default:
		break;
	}
}

float PlayerAbsorbCollOperator::FaceCamera(void)
{
	const Vector3 cam = CurrentCamera::Get().GetAngle();
	const float dx = cam.y - trans.pos.y;
	const float dz = cam.z - trans.pos.z;

	return atan2f(dx, dz);

}