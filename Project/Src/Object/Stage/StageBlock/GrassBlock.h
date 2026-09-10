#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class GrassBlock : public StageBlockBase
{
public:
	GrassBlock(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);
	~GrassBlock()override = default;
	
	void Load(void)override;

private:

};