#pragma once
#include "../../../Common/ActorBase/ActorBase.h"


class Panch: public ActorBase
{
public:
	
	Panch(const Vector3& cactusPos);

	~Panch() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

	// 攻撃開始
	void Start(void);

private:

#pragma region 定数


	// モデル表示倍率
	const float MODEL_SCALE = 1.3f;

	//生成位置の相対座標
	const Vector3 CREATE_LOCAL_POS = Vector3(120.0f, 0.0f, 0.0f);

	// Wait状態の待ち時間
	const unsigned short WAIT_TIME = 60;
	//落下時の最大Y座標
	constexpr static float MAX_FALL_POS_Y = -1500.0f;
	//拡大量
	constexpr static float SCALE_POW = 0.05f;

#pragma endregion

	// Wait状態のカウンター
	unsigned short waitCounter;

#pragma region 参照

	//プレイヤー座標参照
	const Vector3& cactusPos;

#pragma endregion

	void SubUpdate(void) override;
};

