#include "Cactus.h"
#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/CactusWait.h"
#include "State/CactusAttack.h"
#include "State/CactusHit.h"
#include "State/CactusDie.h"

void Cactus::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/Cactus/Cactus");

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
	trans.centerDiff = Vector3(0.0f,- 60.0f,0.0f) * trans.scale;

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	//待機状態追加
	AddState(
		STATE::Wait,
		new CactusWait(
			trans.pos,
			playerPos_,
			[&]() {ChangeState(STATE::Attack); },
			[&]() 
			{
				//アニメーションずれ修正
				trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;
				//再生アニメーション
				AnimePlay((int)ANIM::Hit, true); 
			}
		));

	//HIT状態追加
	AddState(
		STATE::Hit,
		new CactusHit(
			trans.pos,
			[&]()
			{
				//アニメーションずれ修正
				trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;
				//再生アニメーション
				AnimePlay((int)ANIM::Hit, false);
			},
			[&]()
			{
				//アニメーションずれ修正
				trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;

				//再生したアニメが終了したら遷移
				//if (&IsAnimeEnd)
				//{
				//	ChangeState(STATE::Wait);
				//}
		
			}
		));

	AddState(
		STATE::Attack,
		new CactusAttack(
			trans.pos,
			playerPos_,
			[&]() {ChangeState(STATE::Wait); },
			[&]() 
			{
				//アニメーションずれ修正
				trans.centerDiff = Vector3(0.0f, -80.0f, 0.0f) * trans.scale;
				//再生アニメーション
				AnimePlay((int)ANIM::Die, false);
			},
			trans.angle.y
		));

	AddState(
		STATE::Die,
		new CactusDie(
			trans.pos,
			[&]()
			{
				//アニメーションずれ修正
				trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;
				//再生アニメーション
				AnimePlay((int)ANIM::Die, true);
			}
		));


#pragma endregion
}

void Cactus::SubInit(void)
{
	//初期
	ChangeState(STATE::Wait);
}

void Cactus::SubUpdate(void)
{
}

void Cactus::SubDraw(void)
{
}

void Cactus::SubRelease(void)
{
}
