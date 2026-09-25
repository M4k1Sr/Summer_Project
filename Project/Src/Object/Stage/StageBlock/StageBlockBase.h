#pragma once

#include "./StageBlockDefine.h"

#include "../../Common/ActorBase/ActorBase.h"

class StageBlockBase : public ActorBase
{
public:

	StageBlockBase(
		STAGE_BLOCK_TAG BlockTag,
		const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
		);

	~StageBlockBase()override = default;

	void Load(void)override {}
	
protected:

};