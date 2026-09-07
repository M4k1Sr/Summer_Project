#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class DeepGrassBlock : public StageBlockBase
{
public:
	DeepGrassBlock(const Vector3& pos);
	~DeepGrassBlock()override = default;

	void Load(void)override;

private:

};