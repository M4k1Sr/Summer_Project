#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class DirtBlock : public StageBlockBase
{
public:
	DirtBlock(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);
	~DirtBlock()override = default;

	void Load(void)override;

private:

};