#include "Player.h"
#include "../../../Manager/Input/KeyManager.h"
#include "../../../Scene/Common/GameSpace/GameSpaceController.h"
#include "../../../Manager/Camera/CurrentCamera.h"
#include "../../Common/Collider/CapsuleCollider.h"

void Player::CharacterLoad(void)
{
	trans.Load("Player/Player");
	ColliderCreate(new CapsuleCollider(COLLIDER_TAG::PLAYER,
		GetParameterToVector3("Collider","StartPos"), GetParameterToVector3("Collider","EndPos"), GetParameter("Collider","Radius")));


}

void Player::CharacterInit(void)
{
	trans.localAngle.y = Deg2Rad(GetParameter("Init", "angle"));

}

void Player::CharacterUpdate(void)
{
	if (!isOperator) { return; }

	if (CheckHitKey(KEY_INPUT_Z) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault); }

	if (CheckHitKey(KEY_INPUT_X) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane); }

	if (CheckHitKey(KEY_INPUT_C) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::Rail); }

	if (CheckHitKey(KEY_INPUT_V) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::None); }

	const Vector3 moveDirection = GetMoveDirection();

	if (moveDirection != 0.0f) { AddMove(moveDirection * 8.0f); trans.angle.y = atan2f(moveDirection.x, moveDirection.z); }

	// ジャンプと重力・接地判定を確認する
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_JUMP).down && isGround) {
		accelSum.y = 14.0f;
	}
}

void Player::CharacterDraw(void)
{
}

void Player::CharacterAlphaDraw(void)
{
}

void Player::CharacterUiDraw(void)
{
}

void Player::CharacterRelease(void)
{
}

Vector3 Player::GetMoveDirection(void) const
{
	const GameSpaceController& gameSpace = GetGameSpaceController();

	if (gameSpace.IsStopInput()) { return Vector3(); }

	Vector2 input = Key::GetIns().GetLeftStickVec();

	// コントローラー入力がない場合はキーボードを使う
	if (input == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).now) { input.x += 1.0f; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).now) { input.x -= 1.0f; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_UP).now) { input.y += 1.0f; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_DOWN).now) { input.y -= 1.0f; }

		if (input.LengthSq() > 1.0f) { input.Normalize(); }
	}

	return gameSpace.ConvertMoveInput(input, trans.pos, CurrentCamera::Get().GetPos(), GetSpaceConstraint());
}







