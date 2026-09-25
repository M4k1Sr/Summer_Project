#pragma once

#include "./StageBlockDefine.h"

#include "../../Common/ActorBase/ActorBase.h"

class StageBlockBase : public ActorBase
{
public:

	// コンストラクタ
	StageBlockBase(
		STAGE_BLOCK_TAG BlockTag,
		const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
		);

	// デストラクタ
	~StageBlockBase()override = default;

	// 読み込み処理
	void Load(void)override {}
	
protected:

};