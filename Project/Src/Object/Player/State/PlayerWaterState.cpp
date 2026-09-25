#include "PlayerWaterState.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Wepon/Water/PlayerWaterCollOperator.h"

#include "../../Common/Transform/Transform.h"




PlayerWaterState::PlayerWaterState(
	float COLL_START_TIME,
	float COLL_END_TIME,

	const std::vector<PlayerWaterCollOperator*>& collOperators,

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


void PlayerWaterState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down) {
		OwnChangeState();
	}
}

void PlayerWaterState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 攻撃アニメーション再生
	playAnimeAttack();
}

void PlayerWaterState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case PlayerWaterState::STEP::Startup: {
		// 前隙

	// デルタタイム（例: 1/60s）を加算して一定間隔で生成
		spawnTimer += 1.0f / 60.0f;

		if (spawnTimer >= SPAWN_INTERVAL) {
			spawnTimer = 0.0f;

			// 空いているコライダーを探す
			EmitWaterParticle();
		}

		if (COLL_START_TIME <= animeRatio) {
			step = STEP::Active;
		}

		break;
	}

	case PlayerWaterState::STEP::Active: {
		// 攻撃判定発生中

		// 当たり判定を追従

		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

		}

		break;
	}

	case PlayerWaterState::STEP::Recovery: {
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

void PlayerWaterState::Exit(void)
{

}

void PlayerWaterState::EmitWaterParticle()
{
	bool search = false;

	// 未使用のものを探索
	for (auto op : collOperators) {
		if (op->IsAlive()) { continue; }
		search = true;
		op->On();
		break;
	}

	// すべて使用中の場合は一番寿命の短いものを上書き
	if (!search && !collOperators.empty()) {
		size_t overrideIndex = 0;
		for (size_t i = 1; i < collOperators.size(); i++) {
			if (collOperators.at(overrideIndex)->GetLifeTimer() > collOperators.at(i)->GetLifeTimer()) {
				overrideIndex = i;
			}
		}
		collOperators.at(overrideIndex)->On();
	}
}