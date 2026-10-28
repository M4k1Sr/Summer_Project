#include "GobKing.h"

#include "../../../Application/Application.h"

#include "../../../Utility/Utility.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../Common/CharacterBase/CharacterBase.h"

#include "../../Common/Collider/CapsuleCollider.h"

#include "../../Common/Shader/DefaultShader.h"
#include "../../Common/Shader/RimLightShader.h"
#include "../../Common/Shader/WaterShader.h"

#include "../../../Scene/SceneManager.h"

#include "../GobKing/Weapon/CollOperator/GobSwingCollOperator.h"
#include "../GobKing/Weapon/CollOperator/GobThrowCollOperator.h"

#include "State/GobKingIdleState.h"
#include "State/GobKingMoveState.h"
#include "State/GobKingRoarState.h"
#include "State/GobKingSwingState.h"
#include "State/GobKingThrowState.h"
#include "State/GobKingReactState.h"
#include "State/GobKingDieState.h"

void GobKing::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しだしによる重みを設定
	SetPushWeight(PUSH_WEIGHT);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Enemy/Boss/GobKing");

	// モデルのスケール設定
	trans.scale = SCALE;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(CENTER_DIFF) * trans.scale;

	// 初期座標
	trans.pos = Vector3(INIT_POS);

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(ROT_DIFF)));

	//空間制約の設定
	SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane);

#pragma endregion


#pragma region アニメーション読み込み

	// アニメーションコントローラーの生成
	CreateAnimationController();

	// アニメーションの読み込み
	AddInFbxAnimation((int)ANIME_TYPE::Max, ANIME_SPEED_TABLE, ANIME_LOOP_TABLE);

#pragma endregion


#pragma region コライダーの生成

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Enemy,
			Vector3::Yonly(-COLL_START_POS) * trans.scale,
			Vector3::Yonly(-COLL_END_POS) * trans.scale,
			RADIUS * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 下位アクターの生成

	// 攻撃当たり判定管理クラス
	GobSwingCollOperator* swingCollOperator =
		new GobSwingCollOperator(GOB_ATTACK_RADIUS, Vector3(GOB_ATTACK_LOCAL_POS), trans);

	AddChildActor(swingCollOperator);

	// 岩投擲当たり判定管理クラス(現在はまだ未完成)
	GobThrowCollOperator* throwCollOperator =
		new GobThrowCollOperator(GOB_ATTACK_RADIUS, Vector3(GOB_ATTACK_LOCAL_POS), trans);

	AddChildActor(throwCollOperator);

#pragma endregion


#pragma region 状態設定

	// 待機状態
	AddState(
		STATE::Idle,
		new GobKingIdleState(
			SEARCH_RANGE, ATTACK_RANGE,
			trans.pos,
			playerPos,
			[&]() { AnimePlay(ANIME_TYPE::Idle); }
		)
	);

	// おたけび状態
	AddState(
		STATE::Roar,
		new GobKingRoarState(
			HEAR_RANGE,
			trans.pos,
			playerPos,
			[&]() { AnimePlay(ANIME_TYPE::Roar); },
			std::bind(&GobKing::IsAnimeEnd, this),
			[&]() {

				//再生したアニメが終了したら遷移
				if (IsAnimeEnd())
				{
					ChangeState(STATE::Idle);
				}
			},
			[&]() {
				//再生したアニメが終了したら遷移
				if (IsAnimeEnd())
				{
					ChangeState(STATE::Move);
				}

			}
		)
	);

	// 移動状態
	AddState(
		STATE::Move,
		new GobKingMoveState(
			GOB_ACCEL_MAX, SEARCH_RANGE, ATTACK_RANGE,
			std::bind(&GobKing::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,
			trans.pos,
			playerPos,
			[&]() { AnimePlay(ANIME_TYPE::Walk); }
		)
	);

	// 攻撃状態
	AddState(
		STATE::Attack_Swing,
		new GobKingSwingState(
			GOB_COLL_START_TIME,
			GOB_COLL_END_TIME,
			ATTACK_RANGE,
			swingCollOperator,
			trans.pos,
			playerPos,
			[&]() {AnimePlay(ANIME_TYPE::Swing); },
			[&]() {AnimePlay(ANIME_TYPE::Taunt); },
			[&]() {return GetAnimeRatio(); },
			std::bind(&GobKing::IsAnimeEnd, this),
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	// 岩投擲状態
	AddState(
		STATE::Throw_Rock,
		new GobKingThrowState(
			GOB_COLL_START_TIME,
			GOB_COLL_END_TIME,
			ATTACK_RANGE,
			throwCollOperator,
			trans.pos,
			playerPos,
			[&]() {AnimePlay(ANIME_TYPE::Throw); },
			[&]() {AnimePlay(ANIME_TYPE::Taunt); },
			[&]() {return GetAnimeRatio(); },
			std::bind(&GobKing::IsAnimeEnd, this),
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	// リアクション状態
	AddState(
		STATE::React,
		new GobKingReactState(
			hitReact,
			[&]() {AnimePlay(ANIME_TYPE::HitReact); },
			std::bind(&GobKing::IsAnimeEnd, this),
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	// 死亡(吹っ飛び)状態
	AddState(
		STATE::Flying,
		new GobKingDieState(
			isDie,
			isFlyEnd,
			[&]() {AnimePlay(ANIME_TYPE::Flying); },
			std::bind(&GobKing::IsAnimeEnd, this)
		)
	);

#pragma region 待機状態

	// 「待機状態」->「移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Move);
	// 「移動状態」->「待機状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Idle);
	// 「待機状態」->「おたけび状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Roar);
	// 「待機状態」->「攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Attack_Swing);
	// 「待機状態」->「岩投擲攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Throw_Rock);
	// 「待機状態」->「ヒットリアクション状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::React);
	// 「待機状態」->「死亡状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Flying);

#pragma endregion

#pragma region 移動状態

	// 「移動状態」->「攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Attack_Swing);
	// 「移動状態」->「岩投擲攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Throw_Rock);
	// 「移動状態」->「ヒットリアクション状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::React);
	// 「移動状態」->「死亡状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Flying);

#pragma endregion

#pragma region おたけび状態

	// 「おたけび状態」->「ヒットリアクション状態」の自動遷移登録
	RegisterStateTransition(STATE::Roar, STATE::React);
	// 「おたけび状態」->「死亡状態」の自動遷移登録
	RegisterStateTransition(STATE::Roar, STATE::Flying);

#pragma endregion

#pragma region 攻撃状態

	// 「攻撃状態」->「ヒットリアクション状態」の自動遷移登録
	RegisterStateTransition(STATE::Attack_Swing, STATE::React);
	// 「岩投擲攻撃状態」->「ヒットリアクション状態」の自動遷移登録
	RegisterStateTransition(STATE::Throw_Rock, STATE::React);
	// 「攻撃状態」->「死亡状態」の自動遷移登録
	RegisterStateTransition(STATE::Attack_Swing, STATE::Flying);
	// 「岩投擲攻撃状態」->「死亡状態」の自動遷移登録
	RegisterStateTransition(STATE::Throw_Rock, STATE::Flying);

#pragma endregion



#pragma endregion

}

void GobKing::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void GobKing::SubUpdate(void)
{
	// 無敵猶予フレーム処理
	if (invincibleFrame > 0 && TimeScale::Get() > 0.0f) { invincibleFrame--; }

}