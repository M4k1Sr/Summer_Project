#include "Player.h"

#include "../../Utility/Utility.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Camera/CurrentCamera.h"

#include "../../Scene/Common/GameSpace/GameSpaceController.h"

#include "../Common/Collider/CapsuleCollider.h"

void Player::Load(void)
{
	trans.LoadModel("Player/Player");
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::Player,
			GetParameterToVector3("Collider", "StartPos"),
			GetParameterToVector3("Collider", "EndPos"),
			GetParameter("Collider", "Radius")
		)
	);
}

void Player::CharacterInit(void)
{
	trans.localAngle.y = Deg2Rad(GetParameter("Init", "angle"));

	ACCEL_RATE = DECEL_RATE = 3.0f;
	ACCEL_MAX = 15.0f;
}

void Player::CharacterUpdate(void)
{
	if (!isOperator) { return; }

	if (CheckHitKey(KEY_INPUT_Z) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault); }

	if (CheckHitKey(KEY_INPUT_X) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane); }

	if (CheckHitKey(KEY_INPUT_C) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::Rail); }

	if (CheckHitKey(KEY_INPUT_V) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::None); }

	MoveAccel(GetMoveDirection());

	// ジャンプと重力・接地判定を確認する
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down && isGround) {
		velocity.y = 14.0f;
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

	Vector2 input = Input::GetIns().GetLeftStickVec();

	// コントローラー入力がない場合はキーボードを使う
	if (input == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { input.x += 1.0f; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { input.x -= 1.0f; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { input.y += 1.0f; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { input.y -= 1.0f; }

		if (input != 0.0f) { input.Normalize(); }
	}

	return gameSpace.ConvertMoveInput(input, trans.pos, CurrentCamera::Get().GetPos(), GetSpaceConstraint());
}