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

	//バウンドさせる
	void SubOnGrounded(COLLIDER_TAG ownTag, const ColliderBase& other)override;

	// 現在存在しているか
	bool IsAlive(void)const { return lifeTimer > 0.0f; }

	float GetLifeTimer(void)const { return lifeTimer; }



private:

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	//ファイアーボールコライダーの数
	static constexpr int FIREBALL_COLL_NUM = 2;

	// 飛翔時間（秒）
	static constexpr float FIREBALL_LIFETIME_MAX = 2.0f; 

	//移動速度
	static constexpr float SPEED = 10.0f;

	//バウンドの強さ
	static constexpr float BOUNCE_POWER = 5.0f;

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