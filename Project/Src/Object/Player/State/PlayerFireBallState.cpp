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

	step(STEP::Startup)
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

			// 未使用ファイアーボールが見つかったかどうかを記録する一時変数
			bool serch = false;

			// 抱えているファイアーボールを範囲for文で探索していく
			for (auto op : collOperators) {

				// すでに使われているのでこのファイアーボールは使わない
				if (op->IsAlive()) { continue; }

				// 使われていないものが見つかったことを記録する
				serch = true;

				// 発生
				op->On();

				// 1つ発生させて終了
				break;
			}

			// すべてのファイアーボールを探索して、
			// 未使用ファイアーボールが1つも見つからなかった場合
			if (!serch) {

				// パターン1～～～～～～～～～～～～～～～～～～～～～～～～～～

				// 生成失敗として何もしない
				// その方向で行くなら一時変数(serch)ごと消して大丈夫

				// ～～～～～～～～～～～～～～～～～～～～～～～～～～パターン1

				// パターン2～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

				// ファイアーボールの生存タイマー(lifeTimer)の値が1番低い
				// つまり生成して1番時間が経過しているファイアーボールを上書き生成する

				// 上書きする配列添字一時変数（初期値を0とし配列先頭から探索していく）
				size_t overrideIndex = 0;

				// 抱えているファイアーボールをfor文で回す
				for (size_t i = 1; i < collOperators.size(); i++) {
					// 比べていって残りの生存時間が1番短いファイアーボールを探す
					if (collOperators.at(overrideIndex)->GetLifeTimer() > collOperators.at(i)->GetLifeTimer()) {
						overrideIndex = i;
					}
				}

				// 最終的に残りの生存時間が1番短いファイアーボールを上書きするかたちで生成する
				collOperators.at(overrideIndex)->On();

				// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～パターン2
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