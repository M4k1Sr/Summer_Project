#include "PlayerPunchState.h"

#include "../../../Manager/Input/InputManager.h"

#include "../../ColliderOperator/ColliderOperator.h"

PlayerPunchState::PlayerPunchState(const Vector3& playerPos, const float& angleY, ColliderOperator& colliderOperator)
	:playerPos(playerPos), colliderOperator(colliderOperator)
{

	// 攻撃判定の生成位置を計算
	Vector3 forward(
		sinf(angleY),
		0.0f,
		cosf(angleY));

	Vector3 pos =
		Vector3(20.0f, 100.0f, 0.0f) -
		forward * -80;

	// 攻撃判定の生成
	colliderOperator.CreateAttackCollider(
		COLLIDER_TAG::Punch,
		30.0f,
		1,
		0.5f,
		1,
		pos
	);
}

void PlayerPunchState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down) {
		OwnChangeState();
	}
}

void PlayerPunchState::Enter(void)
{

}

void PlayerPunchState::Update(void)
{

}