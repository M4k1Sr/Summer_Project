#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class SphereCollider;

class PlayerWaterCollOperator : public ActorBase
{
public:
	PlayerWaterCollOperator(
		float COLL_RADIUS,
		const Vector3& COLL_LOCAL_POS,

		const Transform& playerTrans
	);
	~PlayerWaterCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 判定発生
	void On(void);
	// 判定消去
	void Off(void);

	//バウンドさせる
	void SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	// 現在存在しているか
	bool IsAlive(void)const { return lifeTimer > 0.0f; }

	float GetLifeTimer(void)const { return lifeTimer; }

	//放水コライダーの数
	static constexpr int WATER_COLL_NUM = 20;

private:

	// 攻撃の判定を発生させる座標
	const Vector3 COLL_LOCAL_POS;

	// 飛翔時間
	static constexpr float WATER_LIFETIME_MAX = 0.8f;

	// 水の落下加速度
	static constexpr float WATER_GRAVITY = 0.5f;

	// 発射方向の上向き成分
	static constexpr float LAUNCH_UP_POWER = 2.0f;

	// 拡散の横方向の幅
	static constexpr float SPREAD_X_RANGE = 0.15f;

	// 拡散の縦方向の幅
	static constexpr float SPREAD_Y_RANGE = 0.10f;

	// 乱数を-0.5〜0.5に中心化するための値
	static constexpr float RAND_CENTER_OFFSET = 0.5f;

	// 水の最低速度
	static constexpr float WATER_SPEED_MIN = 12.0f;

	// 水の速度に加えるランダム幅
	static constexpr float WATER_SPEED_RANGE = 3.0f;

	// 生存タイマー
	float lifeTimer;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 生成するコライダー
	SphereCollider* attackColl;

	//最初に取得した前方
	Vector3 front;

	// 水滴ごとの移動速度ベクトル
	Vector3 waterVec;

	// 更新処理
	void SubUpdate(void)override;
};