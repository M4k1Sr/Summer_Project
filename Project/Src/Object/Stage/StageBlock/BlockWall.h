#pragma once
#include "./StageBlockBase.h"

#include "../../Common/ActorBase/ActorBase.h"

class BlockWall : public StageBlockBase
{
public:

	// コンストラクタ
	BlockWall(const Vector3& pos,
		bool dynamicFlg,
		bool isGravity,
		bool isPushFlg
	);

	// デストラクタ
	~BlockWall()override = default;

	// 読み込み
	void Load(void)override;

private:

	// コライダーサイズ
	const Vector3 COLL_SIZE = VGet(200.0f, 150.0f, 200.0f);
};