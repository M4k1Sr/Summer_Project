#pragma once

#include "../../Common/ActorBase/ActorBase.h"

class StageObject
	: public ActorBase
{
public:

	// コンストラクタ
	StageObject();

	// デストラクタ
	~StageObject()override = default;

	// 読み込み処理
	void Load(void)override;

private:

#pragma region 定数

	// モデルスケール
	const float SCALE = 1.0f;

	// 初期座標
	const Vector3 POS = VGet(7000.0f, -950.0f, 0.0f);

	// コライダーサイズ
	const Vector3 COLL_SIZE = VGet(100.0f, 100.0f, 100.0f);

#pragma endregion

};

