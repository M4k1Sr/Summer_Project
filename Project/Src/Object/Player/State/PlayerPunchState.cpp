#include "PlayerPunchState.h"

#include "../../../Manager/Input/InputManager.h"

PlayerPunchState::PlayerPunchState(const Vector3& playerPos)
	:playerPos(playerPos)
{
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