#include "Player.h"

#include "../../Utility/Utility.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Camera/CurrentCamera.h"

#include "../../Scene/Common/GameSpace/GameSpaceController.h"

#include "../Common/Collider/CapsuleCollider.h"
#include "../Common/Collider/AttackCollider.h"

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerPunchState.h"

#include "Wepon/PlayerPunchCollOperator.h"



Player::Player()
	: CharacterBase("Data/Parameter/Player/"),

	subObjects()
{
}

void Player::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しだしによる重みを設定
	SetPushWeight(50);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Player/Player");

	// モデルのスケール設定
	trans.scale = 1;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -102.81f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.localAngle = Vector3(0.0f, Deg2Rad(180.0f), 0.0f);

#pragma endregion


#pragma region アニメーション読み込み

	// アニメーションコントローラーの生成
	CreateAnimationController();

	// アニメーションの読み込み
	AddInFbxAnimation((int)ANIME_TYPE::Max, ANIME_SPEED_TABLE, ANIME_LOOP_TABLE);

#pragma endregion


#pragma region コライダーの生成

	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::Player,
			Vector3::Yonly(60.0f) * trans.scale,
			Vector3::Yonly(-60.0f) * trans.scale,
			60.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 下位アクターの生成

	// 攻撃当たり判定管理クラス
	PlayerPunchCollOperator* punchCollOperator =
		new PlayerPunchCollOperator(0.0f, Vector3(0, 70, 100), trans);

	subObjects.emplace_back(punchCollOperator);

	for (ActorBase* subObect : subObjects) { subObect->Load(); }
#pragma endregion


#pragma region 状態設定

	// 待機状態
	AddState(
		STATE::Idle,
		new PlayerIdleState([&]() { AnimePlay(ANIME_TYPE::Idle); })
	);

	// 移動状態
	AddState(
		STATE::Move,
		new PlayerMoveState(
			GetGameSpaceController(),
			GetSpaceConstraint(),
			trans.pos,
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			[&]() { AnimePlay(ANIME_TYPE::Walk); },
			[&]() { AnimePlay(ANIME_TYPE::Run); },
			isGround,
			velocity.y
		)
	);


	//// ジャンプ状態
	//AddState(
	//	STATE::Jump,
	//	new PlayerJumpState(
	//		20.0f, velocity.y, isGround,
	//		std::bind(&Player::MoveAccel, this, std::placeholders::_1),
	//		[&]() { AnimePlay(ANIME_TYPE::JumpStart); },
	//		[&]() { AnimePlay(ANIME_TYPE::JumpLoop); },
	//		[&]() { AnimePlay(ANIME_TYPE::Stamp); },
	//		std::bind(&Player::IsAnimeEnd, this),
	//		[&]() { ChangeState(STATE::Idle); }
	//	)
	//);

	// 攻撃（パンチ）状態
	AddState(
		STATE::Punch,
		new PlayerPunchState(
			0.9f, 1.0f,
			*punchCollOperator,
			[&]() { AnimePlay(ANIME_TYPE::Punch,false); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	// 「待機状態」->「移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Move);
	// 「移動状態」->「待機状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Idle);

	//// 「待機状態」->「ジャンプ状態」の自動遷移登録
	//RegisterStateTransition(STATE::Idle, STATE::Jump);
	//// 「移動状態」->「ジャンプ状態」の自動遷移登録
	//RegisterStateTransition(STATE::Move, STATE::Jump);

	// 「待機状態」->「攻撃（パンチ）状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Punch);
	// 「移動状態」->「攻撃（パンチ）状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Punch);

#pragma endregion
}

// 初期化処理
void Player::CharacterInit(void) {
	// モデルの角度のズレを設定
	//trans.localAngle.y = Deg2Rad(GetParameter("Init", "angle"));

	// 加減速度を設定
	ACCEL_RATE = DECEL_RATE = 3.0f;
	// 加速最大値を設定
	ACCEL_MAX = 15.0f;

	// 初期状態を設定
	ChangeState(STATE::Move);
	// 待機状態に遷移
	ChangeState(STATE::Idle);

	// 抱える下位アクター全ての初期化処理
	for (ActorBase* subObject : subObjects) { subObject->Update(); }
}

// 更新処理
void Player::CharacterUpdate(void) {
	if (CheckHitKey(KEY_INPUT_Z) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault); }

	if (CheckHitKey(KEY_INPUT_X) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane); }

	if (CheckHitKey(KEY_INPUT_C) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::Rail); }

	if (CheckHitKey(KEY_INPUT_V) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::None); }
	// 抱える下位アクター全ての更新処理
	for (ActorBase* subObject : subObjects) { subObject->Update(); }
}


void Player::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

std::vector<ColliderBase*> Player::GetCollider(void) const
{
	std::vector<ColliderBase*> ret = {};

	for (ColliderBase* collider : ActorBase::GetCollider()) { ret.emplace_back(collider); }

	for (ActorBase* subObject : subObjects) {
		for (ColliderBase* collider : subObject->GetCollider()) { ret.emplace_back(collider); }
	}

	return ret;
}