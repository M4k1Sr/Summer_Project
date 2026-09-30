#include "CactusHit.h"

CactusHit::CactusHit
(const Vector3& CactusPos,
	std::function<void(void)> playAnimationHit,
	std::function<void(void)> changeStateWait)
	:
	CactusPos(CactusPos),
	playAnimationHit(playAnimationHit),
	changeStateWait(changeStateWait)

{
}

void CactusHit::OwnStateConditionUpdate(void)
{
}

void CactusHit::Update(void)
{
}
