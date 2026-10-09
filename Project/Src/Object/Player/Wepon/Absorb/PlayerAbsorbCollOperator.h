#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class CapsuleCollider;

class PlayerAbsorbCollOperator : public ActorBase
{
public:
	PlayerAbsorbCollOperator(
		std::function<void(const COLLIDER_TAG& tag)> setAbility,
		std::function<void(const float angle)> setAngle,
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

	// 吸収が成功したかどうか
	bool IsAbsorbed(void) const { return isAbsorbed; }

private:

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 生成するコライダー
	CapsuleCollider* attackColl;

	// 吸収が成功したかどうか
	bool isAbsorbed;

	// 能力付与関数
	std::function<void(const COLLIDER_TAG& tag)> setAbility;

	//アニメーション用角度設定関数
	std::function<void(const float angle)> setAngle;

	// 更新処理
	void SubUpdate(void)override;

	//角度計算
	float FaceCamera(void);
};