#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class WallBlock : public StageBlockBase
{
public:
	WallBlock(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);
	~WallBlock()override = default;

	void Load(void)override;

private:

};