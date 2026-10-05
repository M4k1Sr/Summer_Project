#include "Player.h"

#include "../../Utility/Utility.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Camera/CurrentCamera.h"

#include "../../Scene/Common/GameSpace/GameSpaceController.h"

#include "../Common/Collider/CapsuleCollider.h"
#include "../Common/Collider/AttackCollider.h"

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerAbsorbState.h"
#include "State/PlayerPunchState.h"
#include "State/PlayerFireBallState.h"
#include "State/PlayerWaterState.h"
#include "State/PlayerThunderState.h"

#include "Wepon/Punch/PlayerPunchCollOperator.h"
#include "Wepon/FireBall/PlayerFireBallCollOperator.h"
#include "Wepon/Water/PlayerWaterCollOperator.h"
#include "Wepon/Thunder/PlayerThunderCollOperator.h"
#include "Wepon/Absorb/PlayerAbsorbCollOperator.h"



Player::Player()
	: CharacterBase("Data/Parameter/Player/"),
	ability(STATE::Punch)
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
	SetPushWeight(GetParameter("Init", "Weight"));

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Player/Player");

	// モデルのスケール設定
	trans.scale = 1;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, GetParameter("Init", "CenterOffset"), 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.localAngle = Vector3(0.0f, Deg2Rad(GetParameter("Init", "angle")), 0.0f);

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
			Vector3::Yonly(GetParameter("Collider", "CollStart")) * trans.scale,
			Vector3::Yonly(GetParameter("Collider", "CollEnd")) * trans.scale,
			GetParameter("Collider", "Radius") * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 下位アクターの生成

	// 攻撃当たり判定管理クラス

	//吸収
	PlayerAbsorbCollOperator* absorbCollOperator =
		new PlayerAbsorbCollOperator(GetParameter("WeponCollider", "AbsorbCollRadius"),
			GetParameterToVector3("WeponCollider", "AbsorbCollLocalPos"),
			Vector3::Yonly(GetParameter("WeponCollider", "AbsorbCollStart")),
			Vector3::Yonly(GetParameter("WeponCollider", "AbsorbCollEnd")), trans);

	AddChildActor(absorbCollOperator);

	//パンチ
	PlayerPunchCollOperator* punchCollOperator =
		new PlayerPunchCollOperator(GetParameter("WeponCollider", "PunchCollRadius"),
			GetParameterToVector3("WeponCollider", "PunchCollLocalPos"), trans);

	AddChildActor(punchCollOperator);


	//ファイアーボール
	std::vector<PlayerFireBallCollOperator*> fireBallCollOperators;

	fireBallCollOperators.reserve(PlayerFireBallCollOperator::FireBall_COLL_NUM);

	for (int i = 0; i < PlayerFireBallCollOperator::FireBall_COLL_NUM; ++i) {
		auto* fireBallCollOperator = new PlayerFireBallCollOperator(
			GetParameter("WeponCollider", "FireBallCollRadius"),
			GetParameterToVector3("WeponCollider", "FireBallCollLocalPos"), trans);
		AddChildActor(fireBallCollOperator);

		fireBallCollOperators.push_back(fireBallCollOperator);
	}

	//放水
	std::vector<PlayerWaterCollOperator*> waterCollOperators;

	waterCollOperators.reserve(PlayerWaterCollOperator::WATER_COLL_NUM);

	for (int i = 0; i < PlayerWaterCollOperator::WATER_COLL_NUM; ++i) {
		auto* waterCollOperator = new PlayerWaterCollOperator(
			GetParameter("WeponCollider", "WaterCollRadius"),
			GetParameterToVector3("WeponCollider", "WaterCollLocalPos"), trans);
		AddChildActor(waterCollOperator);

		waterCollOperators.push_back(waterCollOperator);
	}

	//サンダー
	std::vector<PlayerThunderCollOperator*> thunderCollOperators;

	thunderCollOperators.reserve(PlayerThunderCollOperator::THUNDER_COLL_NUM);

	for (int i = 0; i < PlayerThunderCollOperator::THUNDER_COLL_NUM; ++i) {
		auto* thunderCollOperator = new PlayerThunderCollOperator(
			GetParameter("WeponCollider", "ThunderCollRadius"),
			GetParameterToVector3("WeponCollider", "ThunderCollLocalPos"), trans);
		AddChildActor(thunderCollOperator);

		thunderCollOperators.push_back(thunderCollOperator);
	}



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

	// 吸収 状態（持続）
	AddState(
		STATE::Absorb,
		new PlayerAbsorbState(
			GetParameter("WeponCollider", "SusActiveTime"),
			GetParameter("WeponCollider", "SusActiveEnd"),
			*absorbCollOperator,
			[&]() { AnimePlay(ANIME_TYPE::Absorb_Start, false); },
			[&]() { AnimePlay(ANIME_TYPE::Absorb_End, false); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::Idle); }
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

	// 攻撃（パンチ）状態 (単発)
	AddState(
		STATE::Punch,
		new PlayerPunchState(
			GetParameter("WeponCollider", "ActiveTime"),
			GetParameter("WeponCollider", "ActiveEnd"),
			*punchCollOperator,
			[&]() {return IsCurrentAbility(STATE::Punch); },
			[&]() { AnimePlay(ANIME_TYPE::Punch,false); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	// 攻撃（ファイアーボール）状態 （単発）
	AddState(
		STATE::FireBall,
		new PlayerFireBallState(
			GetParameter("WeponCollider", "ActiveTime"),
			GetParameter("WeponCollider", "ActiveEnd"),
			fireBallCollOperators,
			[&]() {return IsCurrentAbility(STATE::FireBall); },
			[&]() { AnimePlay(ANIME_TYPE::Punch, false); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	// 攻撃（放水）状態 (持続)
	AddState(
		STATE::Water,
		new PlayerWaterState(
			GetParameter("WeponCollider", "SusActiveTime"),
			GetParameter("WeponCollider", "SusActiveEnd"),
			waterCollOperators,
			[&]() {return IsCurrentAbility(STATE::Water); },
			[&]() { AnimePlay(ANIME_TYPE::Water, false); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	// 攻撃（サンダー）状態 (持続)
	AddState(
		STATE::Thunder,
		new PlayerThunderState(
			GetParameter("WeponCollider", "SusActiveTime"),
			GetParameter("WeponCollider", "SusActiveEnd"),
			thunderCollOperators,
			[&]() {return IsCurrentAbility(STATE::Thunder); },
			[&]() { AnimePlay(ANIME_TYPE::Water, false); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	// 「待機状態」->「移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Move);
	// 「移動状態」->「待機状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Idle);

	// 「待機状態」->「吸収状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Absorb);
	// 「移動状態」->「吸収状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Absorb);

	//// 「待機状態」->「ジャンプ状態」の自動遷移登録
	//RegisterStateTransition(STATE::Idle, STATE::Jump);
	//// 「移動状態」->「ジャンプ状態」の自動遷移登録
	//RegisterStateTransition(STATE::Move, STATE::Jump);

	// 「待機状態」->「攻撃（パンチ）状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Punch);
	// 「移動状態」->「攻撃（パンチ）状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Punch);

	// 「待機状態」->「攻撃（ファイアーボール）状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::FireBall);
	// 「移動状態」->「攻撃（ファイアーボール）状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::FireBall);

	// 「待機状態」->「攻撃（放水）状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Water);
	// 「移動状態」->「攻撃（放水）状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Water);

	// 「待機状態」->「攻撃（サンダー）状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Thunder);
	// 「移動状態」->「攻撃（サンダー）状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Thunder);

#pragma endregion
}

// 初期化処理
void Player::SubInit(void) {

	// 加減速度を設定
	ACCEL_RATE = DECEL_RATE = GetParameter("Init", "Rate");
	// 加速最大値を設定
	ACCEL_MAX = GetParameter("Init", "AccelMax");

	// 初期状態を設定
	ChangeState(STATE::Move);
	// 待機状態に遷移
	ChangeState(STATE::Idle);
	//初期能力を設定
	ability = STATE::FireBall;
}

// 更新処理
void Player::SubUpdate(void) {
	if (CheckHitKey(KEY_INPUT_Z) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault); }

	if (CheckHitKey(KEY_INPUT_X) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane); }

	if (CheckHitKey(KEY_INPUT_C) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::Rail); }

	if (CheckHitKey(KEY_INPUT_V) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::None); }


	if (Input::GetIns().GetInfo(KEY_TYPE::Enter).down) { NextAbility(); }


}


void Player::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Player::NextAbility(void)
{
	const int FIRST = (int)STATE::Punch;
	const int LAST = (int)STATE::Thunder;

	int next = (int)ability + 1;
	if (next > LAST) { next = FIRST; }

	ability = (STATE)next;
}