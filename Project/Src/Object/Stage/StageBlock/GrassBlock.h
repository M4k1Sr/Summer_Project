#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class GrassBlock : public StageBlockBase
{
public:
	GrassBlock(const Vector3& pos);
	~GrassBlock()override = default;
	
	void Load(void)override;

private:

};