#include "NormalSlimeHItState.h"

NormalSlimeHItState::NormalSlimeHItState
(const Vector3& normalSlimePos,
	std::function<void(void)> playAnimationWait)
	:
	normalSlimePos(normalSlimePos),
	playAnimationHit(playAnimationHit)
{
}

void NormalSlimeHItState::OwnStateConditionUpdate(void)
{
}

void NormalSlimeHItState::Update(void)
{

}