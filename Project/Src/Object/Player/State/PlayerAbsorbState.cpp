#include "PlayerAbsorbState.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Wepon/Absorb/PlayerAbsorbCollOperator.h"

#include "../../Common/Transform/Transform.h"


PlayerAbsorbState::PlayerAbsorbState(
	float COLL_START_TIME,
	float COLL_END_TIME, 

	PlayerAbsorbCollOperator& collOperators,


	std::function<void(void)>playAnimeAbsorbStart,
	std::function<void(void)> playAnimeAbsorbEnd,
	std::function<float(void)> getAnimeRatio,
	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	collOperators(collOperators),

	playAnimeAbsorbStart(playAnimeAbsorbStart),
	playAnimeAbsorbEnd(playAnimeAbsorbEnd),
	getAnimeRatio(getAnimeRatio),

	changeStateIdle(changeStateIdle),

	step()
{
}




void PlayerAbsorbState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAbsorb).down) {
		OwnChangeState();
	}
}

void PlayerAbsorbState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 攻撃アニメーション再生
	playAnimeAbsorbStart();
}

void PlayerAbsorbState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case PlayerAbsorbState::STEP::Startup: {
	// 前隙

		collOperators.On();
		if (COLL_START_TIME <= animeRatio) {
			step = STEP::Active;
		}

		break;
	}

	case PlayerAbsorbState::STEP::Active: {
		// 攻撃判定発生中

		// 当たり判定を追従

		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

		}

		break;
	}

	case PlayerAbsorbState::STEP::Recovery: {
		// 後隙

		// アニメーション再生終了で強制的に待機状態へ
		if (1.0f <= animeRatio) {

			collOperators.Off();
			// 待機状態へ遷移
			changeStateIdle();

		}

		break;
	}

	}
}

void PlayerAbsorbState::Exit(void)
{

}

