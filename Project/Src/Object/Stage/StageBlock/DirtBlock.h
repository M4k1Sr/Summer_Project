#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class DirtBlock : public StageBlockBase
{
public:
	// コンストラクタ
	DirtBlock(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);

	// デストラクタ
	~DirtBlock()override = default;

	// 読み込み処理
	void Load(void)override;

};