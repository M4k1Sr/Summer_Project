#include "PlayerFireBallState.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Wepon/FireBall/PlayerFireBallCollOperator.h"

#include "../../Common/Transform/Transform.h"




PlayerFireBallState::PlayerFireBallState(
	float COLL_START_TIME,
	float COLL_END_TIME,

	const std::vector<PlayerFireBallCollOperator*>& collOperators,

	std::function<void(void)> playAnimeAttack,
	std::function<float(void)> getAnimeRatio,

	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	collOperators(collOperators),

	playAnimeAttack(playAnimeAttack),
	getAnimeRatio(getAnimeRatio),

	changeStateIdle(changeStateIdle),

	step()
{
}


void PlayerFireBallState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down) {
		OwnChangeState();
	}
}

void PlayerFireBallState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 当たり判定を消去
	for (auto* op : collOperators) {
		if (op) { op->Off(); }
	}

	// 攻撃アニメーション再生
	playAnimeAttack();
}

void PlayerFireBallState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case PlayerFireBallState::STEP::Startup: {
		// 前隙

		// 攻撃判定発生開始
		if (COLL_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			for (auto* op : collOperators) {
				
				op->On();
			}
		}

		break;
	}

	case PlayerFireBallState::STEP::Active: {
		// 攻撃判定発生中

		// 当たり判定を追従

		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

		}

		break;
	}

	case PlayerFireBallState::STEP::Recovery: {
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

void PlayerFireBallState::Exit(void)
{

}