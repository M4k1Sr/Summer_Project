#include "PlayerThunderState.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Wepon/Thunder/PlayerThunderCollOperator.h"

#include "../../Common/Transform/Transform.h"




PlayerThunderState::PlayerThunderState(
	float COLL_START_TIME,
	float COLL_END_TIME,

	const std::vector<PlayerThunderCollOperator*>& collOperators,

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

	step(STEP::Startup)
{
}


void PlayerThunderState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down) {
		OwnChangeState();
	}
}

void PlayerThunderState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 攻撃アニメーション再生
	playAnimeAttack();

	//初期化
	spawnedCount = 0.0f;
	spawnTimer = 0.0f;
}

void PlayerThunderState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case PlayerThunderState::STEP::Startup: {
		// 前隙

		if (COLL_START_TIME <= animeRatio) {
			step = STEP::Active;
		}

		// 1フレームごとにタイマーを進める
		spawnTimer += 1.0f / 60.0f; // 毎フレーム加算

		// タイマーが間隔（SPAWN_INTERVAL）を超えたら1つ降らせる
		if (spawnTimer >= SPAWN_INTERVAL && spawnedCount < PlayerThunderCollOperator::THUNDER_COLL_NUM) {
			spawnTimer = 0.0f; // タイマーリセット

			// 1つ分生成（何個目の雷かに応じて位置をずらす）
			EmitThunderParticle();
			spawnedCount++;
		}

		break;
	}

	case PlayerThunderState::STEP::Active: {
		// 攻撃判定発生中

	

		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

		}

		break;
	}

	case PlayerThunderState::STEP::Recovery: {
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

void PlayerThunderState::Exit(void)
{

}

void PlayerThunderState::EmitThunderParticle(void)
{
	// 何番目の雷かに応じたオフセット計算（例: 高さと前方距離）
	float frontOffset = (spawnedCount + 1) * THUNDER_DISTANCE_INTERVAL;
	Vector3 offset(0.0f, THUNDER_SPAWN_HEIGHT, frontOffset);

	// 未使用のコライダーを「1つだけ」探してONにする
	for (auto op : collOperators) {
		if (!op->IsAlive()) {
			op->On(offset); // オフセットを渡してON
			return;         // 1つONにしたら関数を抜ける
		}
	}

	// 空きがない場合の上書き処理（必要であれば）
	if (!collOperators.empty()) {
		size_t overrideIndex = 0;
		for (size_t i = 1; i < collOperators.size(); i++) {
			if (collOperators.at(overrideIndex)->GetLifeTimer() > collOperators.at(i)->GetLifeTimer()) {
				overrideIndex = i;
			}
		}
		collOperators.at(overrideIndex)->On(offset);
	}
}