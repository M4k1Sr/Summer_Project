#pragma once

#include <map>

#include "../../Common/ActorBase/ActorBase.h"

class BossStage
	: public ActorBase
{
public:

	// コンストラクタ
	BossStage();

	// デストラクタ
	~BossStage()override = default;

	// 読み込み処理
	void Load(void)override;

private:

#pragma region 定数

	// コライダーサイズ
	const Vector3 COLL_SIZE = VGet(20000.0f, 0.0f, 20000.0f);

	// コライダー座標Y値
	const float COLL_POS_Y = -648.0f;

#pragma endregion 

};

