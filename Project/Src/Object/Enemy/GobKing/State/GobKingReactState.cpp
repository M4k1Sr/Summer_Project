#include "GobKingReactState.h"

#include "../../../../Common/Vector3.h"

GobKingReactState::GobKingReactState(
	bool& hitReact,
	std::function<void(void)> playAnimeReact,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle

) :
	hitReact(hitReact),
	playAnimeReact(playAnimeReact),
	isAnimeEnd(isAnimeEnd),
	changeStateIdle(changeStateIdle)
{
}

void GobKingReactState::OwnStateConditionUpdate(void)
{
	// 攻撃を受けたら遷移
	if (hitReact) {
		OwnChangeState();
	}
}

void GobKingReactState::Enter(void)
{
	hitReact = false;
	playAnimeReact();
}

void GobKingReactState::Update(void)
{
	// アニメーションが終わり次第初期状態
	if (isAnimeEnd()) { changeStateIdle(); }
}

void GobKingReactState::Exit(void)
{
	// 状態を抜けるときに判定をfalse
	hitReact = false;
}
