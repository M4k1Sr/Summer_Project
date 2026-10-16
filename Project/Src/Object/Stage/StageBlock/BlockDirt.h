#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class BlockDirt : public StageBlockBase
{
public:
	BlockDirt(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);
	~BlockDirt()override = default;

	void Load(void)override;

private:

};