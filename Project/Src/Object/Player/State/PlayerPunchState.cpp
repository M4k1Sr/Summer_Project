#include "PlayerPunchState.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Wepon/PlayerPunchCollOperator.h"

#include "../../Common/Transform/Transform.h"




PlayerPunchState::PlayerPunchState(
	float COLL_START_TIME,
	float COLL_END_TIME,

	PlayerPunchCollOperator& collOperator,

	std::function<void(void)> playAnimeAttack,
	std::function<float(void)> getAnimeRatio,

	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	collOperator(collOperator),

	playAnimeAttack(playAnimeAttack),
	getAnimeRatio(getAnimeRatio),

	changeStateIdle(changeStateIdle),

	step()
{
}


void PlayerPunchState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down) {
		OwnChangeState();
	}
}

void PlayerPunchState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 当たり判定を消去
	collOperator.Off();

	// 攻撃アニメーション再生
	playAnimeAttack();
}

void PlayerPunchState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case PlayerPunchState::STEP::Startup: {
		// 前隙

		// 攻撃判定発生開始
		if (COLL_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			collOperator.On();
		}

		break;
	}

	case PlayerPunchState::STEP::Active: {
		// 攻撃判定発生中

		// 当たり判定を追従

		// 攻撃判定発生終了
		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 当たり判定を消去
			collOperator.Off();
		}

		break;
	}

	case PlayerPunchState::STEP::Recovery: {
		// 後隙

		// アニメーション再生終了で強制的に待機状態へ
		if (1.0f <= animeRatio) {

			// 待機状態へ遷移
			changeStateIdle();
		}

		break;
	}

	}
}

void PlayerPunchState::Exit(void)
{
	// 当たり判定を消去
	collOperator.Off();
}