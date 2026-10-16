#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class BlockGrass : public StageBlockBase
{
public:
	BlockGrass(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);
	~BlockGrass()override = default;
	
	void Load(void)override;

private:

};