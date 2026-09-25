#pragma once
#include "./StageBlockBase.h"
#include "../../Common/ActorBase/ActorBase.h"

class GrassBlock : public StageBlockBase
{
public:
	// コンストラクタ
	GrassBlock(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);

	// デストラクタ
	~GrassBlock()override = default;
	
	// 読み込み処理
	void Load(void)override;

};