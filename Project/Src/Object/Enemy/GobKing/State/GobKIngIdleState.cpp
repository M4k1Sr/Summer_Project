#include "GobKingIdleState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"

GobKingIdleState::GobKingIdleState(
	float SEARCH_RANGE,
	float ATTACK_RANGE,

	const Vector3& gobPos,
	const Vector3*& playerPos,
	std::function<void(void)> playAnimeIdle
) :
	gobPos(gobPos),
	playerPos(playerPos),

	SEARCH_RANGE(SEARCH_RANGE),
	ATTACK_RANGE(ATTACK_RANGE),

	playAnimeIdle(playAnimeIdle)
{
}

void GobKingIdleState::OwnStateConditionUpdate(void)
{
	// プレイヤーがいない場合もIdle状態
	if (playerPos == nullptr) { OwnChangeState(); return; }

	Vector3 diff = *playerPos - gobPos;
	float distSq = diff.x * diff.x + diff.z * diff.z;

	// 索敵範囲の外、または攻撃範囲の内側 → 待機
	if (distSq >= SEARCH_RANGE * SEARCH_RANGE)
	{
		OwnChangeState();
		return;
	}
}

void GobKingIdleState::Enter(void)
{
	playAnimeIdle();
}