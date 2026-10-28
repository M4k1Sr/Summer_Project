#pragma once
#include "./StageBlockBase.h"

#include "../../Common/ActorBase/ActorBase.h"

class BlockGrass : public StageBlockBase
{
public:

	// コンストラクタ
	BlockGrass(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);

	// デストラクタ
	~BlockGrass()override = default;

	// 読み込み
	void Load(void)override;

private:

	// コライダーサイズ
	const Vector3 COLL_SIZE = VGet(200.0f, 150.0f, 600.0f);
};