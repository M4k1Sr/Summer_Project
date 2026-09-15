#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class SphereCollider;

class PlayerFireBallCollOperator : public ActorBase
{
public:
	PlayerFireBallCollOperator(
		float COLL_RADIUS,
		const Vector3& COLL_LOCAL_POS,

		const Transform& playerTrans
	);
	~PlayerFireBallCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 判定発生
	void On(void);
	// 判定消去
	void Off(void);

private:

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// 飛翔時間（秒）
	static constexpr float FIREBALL_LIFETIME_MAX = 2.0f; 
	// 生存タイマー
	float lifeTimer;                             

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 生成するコライダー
	SphereCollider* attackColl;

	//最初に取得した前方
	Vector3 front;

	// 更新処理
	void SubUpdate(void)override;
};