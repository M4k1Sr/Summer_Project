#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class CapsuleCollider;

class PlayerAbsorbCollOperator : public ActorBase
{
public:
	PlayerAbsorbCollOperator(
		float COLL_RADIUS,
		const Vector3& COLL_LOCAL_POS,
		const Vector3& COLL_START_POS,
		const Vector3& COLL_END_POS,

		const Transform& playerTrans
	);
	~PlayerAbsorbCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 判定発生
	void On(void);
	// 判定消去
	void Off(void);

private:

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 生成するコライダー
	CapsuleCollider* attackColl;

	// 更新処理
	void SubUpdate(void)override;
};