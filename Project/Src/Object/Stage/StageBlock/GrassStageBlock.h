#pragma once

#include "../../Common/ActorBase/ActorBase.h"

class GrassStageBlock : public ActorBase
{
public:
	GrassStageBlock(const Vector3& pos);
	~GrassStageBlock()override = default;
	
	void Load(void)override;

private:

};