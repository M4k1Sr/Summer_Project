#pragma once

#include <map>

#include "../../Common/ActorBase/ActorBase.h"

class BossStage
	: public ActorBase
{
public:
	BossStage();
	~BossStage()override = default;

	// “Ç‚İ‚İˆ—
	void Load(void)override;

private:

	// XV
	void SubUpdate(void)override;
};

