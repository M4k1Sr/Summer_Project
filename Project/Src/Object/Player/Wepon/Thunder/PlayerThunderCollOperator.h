#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class SphereCollider;

class PlayerThunderCollOperator : public ActorBase
{
public:
	PlayerThunderCollOperator(
		float COLL_RADIUS,
		const Vector3& COLL_LOCAL_POS,

		const Transform& playerTrans
	);
	~PlayerThunderCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 判定発生
	void On(const Vector3& spawnOffset);
	// 判定消去
	void Off(void);

	//バウンドさせる
	void SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	// 現在存在しているか
	bool IsAlive(void)const { return lifeTimer > 0.0f; }

	float GetLifeTimer(void)const { return lifeTimer; }

	//サンダーコライダーの数
	static constexpr int THUNDER_COLL_NUM = 3;

private:

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// 飛翔時間（秒）
	static constexpr float WATER_LIFETIME_MAX = 0.8f;
	// 生存タイマー
	float lifeTimer;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 生成するコライダー
	SphereCollider* attackColl;

	//最初に取得した前方
	Vector3 front;

	// 水滴ごとの移動速度ベクトル
	Vector3 velocity_;

	// 更新処理
	void SubUpdate(void)override;
};