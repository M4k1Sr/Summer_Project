#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class DirtBlock : public StageBlockBase
{
public:
	DirtBlock(const Vector3& pos);
	~DirtBlock()override = default;

	void Load(void)override;

private:

};