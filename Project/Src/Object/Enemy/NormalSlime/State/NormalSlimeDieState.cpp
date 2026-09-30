#include "NormalSlimeDieState.h"

NormalSlimeDieState::NormalSlimeDieState
(const Vector3& normalSlimePos, 
	std::function<void(void)> playAnimationDie)
	:
	normalSlimePos(normalSlimePos),
	playAnimationDie(playAnimationDie)
{
}

void NormalSlimeDieState::OwnStateConditionUpdate(void)
{
}

void NormalSlimeDieState::Update(void)
{
}
