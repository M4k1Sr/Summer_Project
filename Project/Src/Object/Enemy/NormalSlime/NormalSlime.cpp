#include "NormalSlime.h"
#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/NormalSlimeMoveState.h"
#include "State/NormalSlimeHItState.h"
#include "State/NormalSlimeDieState.h"

NormalSlime::NormalSlime(const Vector3& pos, int hp)
	:
	CharacterBase(),
	initPos(pos)
{
	SetMaxHp(hp);
	trans.pos = initPos;
	SetGravityFlg(true);
	//SetPushFlg(false);
	//SetIsDraw(true);
}

void NormalSlime::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/NormalSlime/NormalSlime");

	//アニメーション追加
	CreateAnimationController();
	//アニメーション登録
	AddInFbxAnimation((int)ANIM::MAX, animationSpeedTabel_);

	//ステージ・プレイヤー用コライダー
	AddCollider(new SphereCollider(COLLIDER_TAG::Enemy, COLL_SIZE));
	//攻撃判定用コライダー
//	AddCollider(new SphereCollider(COLLIDER_TAG::EnemyAttackBox, HIT_BOX_SIZE));

	//空間制約の設定
	SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane);

	//モデルの角度のズレを設定
	trans.localRotation = Quaternion::FromRotationY(Deg2Rad(ADJUSTMENT_ANGLE));

	//サイズ設定
	trans.scale = ADJUSTMENT_SIZE;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -50.0f, 0.0f) * trans.scale;

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	// 移動状態を追加
	AddState(
		STATE::Move,
		new NormalSlimeMoveState(
			trans.pos,
			initPos,
			std::bind(&NormalSlime::MoveAccel, this, std::placeholders::_1)
			));

	// 移動状態を追加
	AddState(
		STATE::Hit,
		new NormalSlimeHItState(
			trans.pos,
			[&]()
			{
				//再生アニメーション
				AnimePlay((int)ANIM::Hit, false);
			}
		));

	// 死亡状態を追加
	AddState(
		STATE::Die,
		new NormalSlimeDieState(
			trans.pos,
			[&]() {AnimePlay((int)ANIM::Die, false); },
			[&]() {return GetAlive(); }
		));

	// 「移動状態」->「死亡状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Die);

#pragma endregion
}

void NormalSlime::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{

}

void NormalSlime::SubInit(void)
{
	//初期ステート
	ChangeState(STATE::Move);

	//初期再生アニメーション
	AnimePlay((int)ANIM::Idle, true);

	// 加減速度を設定
	ACCEL_RATE = DECEL_RATE = 3.0f;
	// 加速最大値を設定
	ACCEL_MAX = 5.0f;
}

void NormalSlime::SubUpdate(void)
{
}

void NormalSlime::SubDraw(void)
{
}

void NormalSlime::SubRelease(void)
{
}
