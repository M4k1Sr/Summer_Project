#pragma once

#include <map>

#include "../../Common/ActorBase/ActorBase.h"

class StageObject
	: public ActorBase
{
public:
	StageObject();
	~StageObject()override = default;

	// “Ç‚İ‚İˆ—
	void Load(void)override;

private:

	// XV
	void SubUpdate(void)override;
};

