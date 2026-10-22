#include "NormalSlimeDieState.h"

NormalSlimeDieState::NormalSlimeDieState
(const Vector3& normalSlimePos, 
	std::function<void(void)> playAnimationDie,
	std::function<bool(void)> isDead
)	:
	normalSlimePos(normalSlimePos),
	playAnimationDie(playAnimationDie),
	isDead(isDead)
{
}

void NormalSlimeDieState::OwnStateConditionUpdate(void)
{
	if (!isDead()) { return; }

	OwnChangeState();
}

void NormalSlimeDieState::Update(void)
{
}
