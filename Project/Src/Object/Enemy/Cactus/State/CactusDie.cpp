#include "CactusDie.h"

CactusDie::CactusDie(const Vector3& CactusPos, 
	std::function<void(void)> playAnimationDie)
	:
	CactusPos(CactusPos),
	playAnimationDie(playAnimationDie)
{
}

void CactusDie::OwnStateConditionUpdate(void)
{
}

void CactusDie::Update(void)
{
}
