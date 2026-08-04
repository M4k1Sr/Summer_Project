#pragma once

#include "../../ActorBase.h"

#include "../../../Manager/Input/KeyManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"
#include "../../../Scene/Common/GameSpace/GameSpaceController.h"

class DebugObjectBase : public ActorBase
{
public:
	DebugObjectBase(
		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight,

		bool isOperator
	) :
		ActorBase(),
		isOperator(isOperator)
	{
		trans.pos = pos;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}
	DebugObjectBase(
		const std::string& parameterPath,

		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight,

		bool isOperator
	) :
		ActorBase(parameterPath),
		INIT_POS(pos),
		isOperator(isOperator)
	{
		trans.pos = INIT_POS;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}
	~DebugObjectBase()override = default;

private:

	// 操作可能かどうか
	bool isOperator;

	// 初期座標
	const Vector3 INIT_POS;

	Vector3 GetMoveDirection(void)const {
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

	void ResetPos(void) { trans.pos = INIT_POS; }


	void SubUpdate(void)override {
		if (!isOperator) { return; }

		if (CheckHitKey(KEY_INPUT_Z) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault); }

		if (CheckHitKey(KEY_INPUT_X) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane); }

		if (CheckHitKey(KEY_INPUT_C) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::Rail); }

		if (CheckHitKey(KEY_INPUT_V) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::None); }

		const Vector3 moveDirection = GetMoveDirection();

		if (moveDirection != 0.0f) { AddMove(moveDirection * 8.0f); trans.angle.y = atan2f(moveDirection.x, moveDirection.z); }

		// ジャンプと重力・接地判定を確認する
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_JUMP).down && isGround) { accelSum.y = 14.0f; }

		if (CheckHitKey(KEY_INPUT_R) != 0) { ResetPos(); }
	}
};