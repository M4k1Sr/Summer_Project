#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class BlockWall : public StageBlockBase
{
public:
	BlockWall(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);
	~BlockWall()override = default;

	void Load(void)override;

private:

};