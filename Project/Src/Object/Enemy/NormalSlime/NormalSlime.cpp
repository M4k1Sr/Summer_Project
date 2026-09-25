#include "NormalSlime.h"
#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/NormalSlimeMoveState.h"
#include "State/NormalSlimeHItState.h"
#include "State/NormalSlimeDieState.h"

void NormalSlime::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/NormalSlime/NormalSlime");

	//アニメーション追加
	CreateAnimationController();
	//アニメーション登録
	AddInFbxAnimation((int)ANIM::MAX, animationSpeedTabel_);

	ColliderCreate(new SphereCollider(COLLIDER_TAG::Enemy, COLL_SIZE));

	//モデルの角度のズレを設定
	trans.localAngle.y = Deg2Rad(ADJUSTMENT_ANGLE);

	//サイズ設定
	trans.scale = ADJUSTMENT_SIZE;

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	// 移動状態を追加
	AddState(
		STATE::Move,
		new NormalSlimeMoveState(
			trans.pos,
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

	// 移動状態を追加
	AddState(
		STATE::Die,
		new NormalSlimeDieState(
			trans.pos,
			[&]()
			{
				//再生アニメーション
				AnimePlay((int)ANIM::Die, false);
			}
		));

#pragma endregion
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
	ACCEL_MAX = 7.0f;
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
