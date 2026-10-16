#include "Thorn.h"

#include "Wepon/Icicle.h"

#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/ThornAttack.h"
#include "State/ThornWait.h"

void Thorn::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/Thorn/Thorn");

	//アニメーション追加
	CreateAnimationController();
	//アニメーション登録
	AddInFbxAnimation((int)ANIM::MAX, animationSpeedTabel_);

	//コライダー生成
	ColliderCreate(new SphereCollider(COLLIDER_TAG::Enemy, COLL_SIZE));
	//攻撃判定用コライダー
	ColliderCreate(new SphereCollider(COLLIDER_TAG::EnemyHitBox, HIT_BOX_SIZE));

	//空間製薬の設定
	SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane);

	// モデルの角度のズレを設定
	trans.localAngle.y = Deg2Rad(180.0f);
	
	//サイズ設定
	trans.scale = 1.3f;

	//アニメーションずれ修正
	trans.centerDiff = Vector3(0.0f, -40.0f, 0.0f) * trans.scale;

	//プレイヤー座標参照を取得
	Icicle* icicle = new Icicle(playerPos);
	AddChildActor(icicle);

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	// 移動状態を追加
	AddState(
		STATE::Attack,
		new ThornAttack(
			trans.pos,
			playerPos,
			[&]()
			{
				ChangeState(STATE::Wait);
			},
			[&]()
			{
				//再生アニメーション
				AnimePlay((int)ANIM::Attack, false);
			},
			trans.angle.y,
			*icicle
			));

	//待機状態を追加
	AddState(
		STATE::Wait,
		new ThornWait(
			trans.pos,
			playerPos,
			[&]()
			{
				ChangeState(STATE::Attack);
			},
			[&]()
			{
				//再生アニメーション
				AnimePlay((int)ANIM::Wait, true);
			}
		));
	

#pragma endregion
}

void Thorn::SubInit(void)
{
	ChangeState(STATE::Wait);

	// 加減速度を設定
	ACCEL_RATE = DECEL_RATE = 0.5f;
	// 加速最大値を設定
	ACCEL_MAX = 4.0f;
}