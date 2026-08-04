#include "KeyManager.h"

#include <algorithm>

#include "../../Utility/Utility.h"
#include "../../Application/Application.h"

KeyManager* KeyManager::ins = nullptr;

KeyManager::KeyManager():
	keyInfo(),
	keyboardFormat(),
	mouseButtonFormat(),
	controllerButtonFormat(),
	mouseFixed(false),

	lastInputKinds(false)
{
}

void KeyManager::Init(void)
{
	// キーボードを割り振るとき
#define SET_KEYBOARD(type,key)keyboardFormat[(int)type].emplace_back(key)

	// コントローラーのボタンを割り振るとき
#define SET_C_BUTTON(type,key)controllerButtonFormat[(int)type].emplace_back(key)

	// マウスのボタンを割り振るとき
#define SET_MOUSE_BUTTON(type,key)mouseButtonFormat[(int)type].emplace_back(key)

	// コントローラーのボタン以外(スティックやトリガーなど)を割り振るとき
#define SET_C_OTHERS(type,key)controllerOthersFormat[(int)type].emplace_back(key)

#pragma region プレイヤー操作
	// 移動
	SET_KEYBOARD(KEY_TYPE::PLAYER_MOVE_UP, KEY_INPUT_W);
	SET_C_BUTTON(KEY_TYPE::PLAYER_MOVE_UP, XINPUT_BUTTON_DPAD_UP);
	SET_C_OTHERS(KEY_TYPE::PLAYER_MOVE_UP, CONTROLLER_OTHERS::LEFTSTICK_UP);

	SET_KEYBOARD(KEY_TYPE::PLAYER_MOVE_DOWN, KEY_INPUT_S);
	SET_C_BUTTON(KEY_TYPE::PLAYER_MOVE_DOWN, XINPUT_BUTTON_DPAD_DOWN);
	SET_C_OTHERS(KEY_TYPE::PLAYER_MOVE_DOWN, CONTROLLER_OTHERS::LEFTSTICK_DOWN);

	SET_KEYBOARD(KEY_TYPE::PLAYER_MOVE_RIGHT, KEY_INPUT_D);
	SET_C_BUTTON(KEY_TYPE::PLAYER_MOVE_RIGHT, XINPUT_BUTTON_DPAD_RIGHT);
	SET_C_OTHERS(KEY_TYPE::PLAYER_MOVE_RIGHT, CONTROLLER_OTHERS::LEFTSTICK_RIGHT);

	SET_KEYBOARD(KEY_TYPE::PLAYER_MOVE_LEFT, KEY_INPUT_A);
	SET_C_BUTTON(KEY_TYPE::PLAYER_MOVE_LEFT, XINPUT_BUTTON_DPAD_LEFT);
	SET_C_OTHERS(KEY_TYPE::PLAYER_MOVE_LEFT, CONTROLLER_OTHERS::LEFTSTICK_LEFT);

	// ダッシュ
	SET_KEYBOARD(KEY_TYPE::PLAYER_DASH, KEY_INPUT_LSHIFT);
	SET_C_BUTTON(KEY_TYPE::PLAYER_DASH, XINPUT_BUTTON_LEFT_THUMB);
	SET_C_BUTTON(KEY_TYPE::PLAYER_DASH, XINPUT_BUTTON_A);

	// ジャンプ
	SET_KEYBOARD(KEY_TYPE::PLAYER_JUMP, KEY_INPUT_SPACE);
	SET_C_BUTTON(KEY_TYPE::PLAYER_JUMP, XINPUT_BUTTON_A);

#pragma endregion

#pragma region カメラ操作
	// 移動
	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_FRONT, KEY_INPUT_I);
	SET_C_OTHERS(KEY_TYPE::CAMERA_MOVE_FRONT, CONTROLLER_OTHERS::LEFTSTICK_UP);

	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_BACK, KEY_INPUT_K);
	SET_C_OTHERS(KEY_TYPE::CAMERA_MOVE_BACK, CONTROLLER_OTHERS::LEFTSTICK_DOWN);

	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_RIGHT, KEY_INPUT_L);
	SET_C_OTHERS(KEY_TYPE::CAMERA_MOVE_RIGHT, CONTROLLER_OTHERS::LEFTSTICK_RIGHT);

	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_LEFT, KEY_INPUT_J);
	SET_C_OTHERS(KEY_TYPE::CAMERA_MOVE_LEFT, CONTROLLER_OTHERS::LEFTSTICK_LEFT);

	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_UP, KEY_INPUT_M);
	SET_C_BUTTON(KEY_TYPE::CAMERA_MOVE_UP, XINPUT_BUTTON_DPAD_UP);

	SET_KEYBOARD(KEY_TYPE::CAMERA_MOVE_DOWN, KEY_INPUT_N);
	SET_C_BUTTON(KEY_TYPE::CAMERA_MOVE_DOWN, XINPUT_BUTTON_DPAD_DOWN);

	// 回転
	SET_KEYBOARD(KEY_TYPE::CAMERA_ROT_FRONT, KEY_INPUT_UP);
	SET_C_OTHERS(KEY_TYPE::CAMERA_ROT_FRONT, CONTROLLER_OTHERS::RIGHTSTICK_UP);

	SET_KEYBOARD(KEY_TYPE::CAMERA_ROT_BACK, KEY_INPUT_DOWN);
	SET_C_OTHERS(KEY_TYPE::CAMERA_ROT_BACK, CONTROLLER_OTHERS::RIGHTSTICK_DOWN);

	SET_KEYBOARD(KEY_TYPE::CAMERA_ROT_RIGHT, KEY_INPUT_E);
	SET_KEYBOARD(KEY_TYPE::CAMERA_ROT_RIGHT, KEY_INPUT_O);
	SET_C_BUTTON(KEY_TYPE::CAMERA_ROT_RIGHT, XINPUT_BUTTON_RIGHT_SHOULDER);

	SET_KEYBOARD(KEY_TYPE::CAMERA_ROT_LEFT, KEY_INPUT_Q);
	SET_KEYBOARD(KEY_TYPE::CAMERA_ROT_LEFT, KEY_INPUT_U);
	SET_C_BUTTON(KEY_TYPE::CAMERA_ROT_LEFT, XINPUT_BUTTON_LEFT_SHOULDER);

#pragma endregion


#pragma region システム操作
	// 汎用（ポーズ/終了）キー
	SET_KEYBOARD(KEY_TYPE::PAUSE, KEY_INPUT_ESCAPE);
	SET_C_BUTTON(KEY_TYPE::PAUSE, XINPUT_BUTTON_START);

	// 汎用決定キー
	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_SPACE);
	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_RETURN);
	SET_C_BUTTON(KEY_TYPE::ENTER, XINPUT_BUTTON_B);
	SET_C_BUTTON(KEY_TYPE::ENTER, XINPUT_BUTTON_Y);

	// 汎用方向キー
	SET_KEYBOARD(KEY_TYPE::UP, KEY_INPUT_W);
	SET_KEYBOARD(KEY_TYPE::UP, KEY_INPUT_UP);
	SET_C_BUTTON(KEY_TYPE::UP, XINPUT_BUTTON_DPAD_UP);
	SET_C_OTHERS(KEY_TYPE::UP, CONTROLLER_OTHERS::LEFTSTICK_UP);
	SET_C_OTHERS(KEY_TYPE::UP, CONTROLLER_OTHERS::RIGHTSTICK_UP);

	SET_KEYBOARD(KEY_TYPE::DOWN, KEY_INPUT_S);
	SET_KEYBOARD(KEY_TYPE::DOWN, KEY_INPUT_DOWN);
	SET_C_BUTTON(KEY_TYPE::DOWN, XINPUT_BUTTON_DPAD_DOWN);
	SET_C_OTHERS(KEY_TYPE::DOWN, CONTROLLER_OTHERS::LEFTSTICK_DOWN);
	SET_C_OTHERS(KEY_TYPE::DOWN, CONTROLLER_OTHERS::RIGHTSTICK_DOWN);

	SET_KEYBOARD(KEY_TYPE::RIGHT, KEY_INPUT_D);
	SET_KEYBOARD(KEY_TYPE::RIGHT, KEY_INPUT_RIGHT);
	SET_C_BUTTON(KEY_TYPE::RIGHT, XINPUT_BUTTON_DPAD_RIGHT);
	SET_C_OTHERS(KEY_TYPE::RIGHT, CONTROLLER_OTHERS::LEFTSTICK_RIGHT);
	SET_C_OTHERS(KEY_TYPE::RIGHT, CONTROLLER_OTHERS::RIGHTSTICK_RIGHT);

	SET_KEYBOARD(KEY_TYPE::LEFT, KEY_INPUT_A);
	SET_KEYBOARD(KEY_TYPE::LEFT, KEY_INPUT_LEFT);
	SET_C_BUTTON(KEY_TYPE::LEFT, XINPUT_BUTTON_DPAD_LEFT);
	SET_C_OTHERS(KEY_TYPE::LEFT, CONTROLLER_OTHERS::LEFTSTICK_LEFT);
	SET_C_OTHERS(KEY_TYPE::LEFT, CONTROLLER_OTHERS::RIGHTSTICK_LEFT);

	SET_KEYBOARD(KEY_TYPE::TEXT_INPUT_LANGUAGE_SWITCH, KEY_INPUT_LSHIFT);
	SET_KEYBOARD(KEY_TYPE::TEXT_INPUT_LANGUAGE_SWITCH, KEY_INPUT_RSHIFT);
#pragma endregion


#pragma region デバッグキー
	// デバッグオブジェクト操作
	SET_KEYBOARD(KEY_TYPE::DEBUG_OBJECT_FRONT, KEY_INPUT_T);
	SET_KEYBOARD(KEY_TYPE::DEBUG_OBJECT_BACK, KEY_INPUT_G);
	SET_KEYBOARD(KEY_TYPE::DEBUG_OBJECT_RIGHT, KEY_INPUT_H);
	SET_KEYBOARD(KEY_TYPE::DEBUG_OBJECT_LEFT, KEY_INPUT_F);
	SET_KEYBOARD(KEY_TYPE::DEBUG_OBJECT_UP, KEY_INPUT_Y);
	SET_KEYBOARD(KEY_TYPE::DEBUG_OBJECT_DOWN, KEY_INPUT_R);

	// デバッグ描画スイッチ
	SET_KEYBOARD(KEY_TYPE::DEBUG_DRAW_SWITCH, KEY_INPUT_8);

	// デバッグシーンの切り替え
	SET_KEYBOARD(KEY_TYPE::DEBUG_MODE_SWITCH, KEY_INPUT_9);

	// デバッグモード時元のシーンの更新
	SET_KEYBOARD(KEY_TYPE::DEBUG_MODE_TOPUPDATE, KEY_INPUT_0);

	// シーンの再ロード
	SET_KEYBOARD(KEY_TYPE::DEBUG_RELOAD, KEY_INPUT_MINUS);

#pragma endregion
}

void KeyManager::Update(void)
{
	// XInput状態を先に1回だけ取得し、以降の入力判定で使い回す
	ControllerUpdate();

	KeyUpdate();
	MouseUpdate();
}

void KeyManager::Release(void)
{
	for (auto& input : keyboardFormat) { input.clear(); }			keyboardFormat->clear();
	for (auto& input : controllerButtonFormat) { input.clear(); }	controllerButtonFormat->clear();
	for (auto& input : mouseButtonFormat) { input.clear(); }		mouseButtonFormat->clear();
	for (auto& input : controllerOthersFormat) { input.clear(); }	controllerOthersFormat->clear();
}

void KeyManager::ControllerUpdate(void)
{
	controllerState = {};

	controllerConnected =
		(GetJoypadXInputState(
			DX_INPUT_PAD1,
			&controllerState
		) == 0);
}

void KeyManager::KeyUpdate(void)
{
	for (int i = 0; i < (int)KEY_TYPE::MAX; i++) {
		keyInfo[i].prev = keyInfo[i].now;

		bool b = false;

		for (auto& input : keyboardFormat[i]) {
			if (b) { break; }

			if (CheckHitKey(input) != 0) { b = true; lastInputKinds = false; }
		}
		for (auto& input : controllerButtonFormat[i]) {
			if (b) { break; }

			XINPUT_STATE state = {};
			if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { state = {}; }

			if (state.Buttons[input] != 0) { b = true; lastInputKinds = true; }
		}
		for (auto& input : mouseButtonFormat[i]) {
			if (b) { break; }
			if (GetMouseInput() & input) { b = true; lastInputKinds = false; }
		}
		for (CONTROLLER_OTHERS input : controllerOthersFormat[i]) {
			if (b) { break; }
			if (ControllerOthersInput(input)) { b = true; lastInputKinds = true; }
		}

		keyInfo[i].now = b;

		keyInfo[i].up = (keyInfo[i].prev && !keyInfo[i].now);
		keyInfo[i].down = (!keyInfo[i].prev && keyInfo[i].now);
	}
}

bool KeyManager::ControllerOthersInput(const CONTROLLER_OTHERS& input)const
{
	if (!controllerConnected) { return false; }

	const Vector2 leftStick = GetLeftStickVec();
	const Vector2 rightStick = GetRightStickVec();

	switch (input)
	{
	case CONTROLLER_OTHERS::LEFTSTICK_UP:
		return leftStick.y > 0.0f;

	case CONTROLLER_OTHERS::LEFTSTICK_DOWN:
		return leftStick.y < 0.0f;

	case CONTROLLER_OTHERS::LEFTSTICK_RIGHT:
		return leftStick.x > 0.0f;

	case CONTROLLER_OTHERS::LEFTSTICK_LEFT:
		return leftStick.x < 0.0f;

	case CONTROLLER_OTHERS::RIGHTSTICK_UP:
		return rightStick.y > 0.0f;

	case CONTROLLER_OTHERS::RIGHTSTICK_DOWN:
		return rightStick.y < 0.0f;

	case CONTROLLER_OTHERS::RIGHTSTICK_RIGHT:
		return rightStick.x > 0.0f;

	case CONTROLLER_OTHERS::RIGHTSTICK_LEFT:
		return rightStick.x < 0.0f;

	case CONTROLLER_OTHERS::LEFT_TRIGGER:
		return GetLeftTriggerValue() > 0.0f;

	case CONTROLLER_OTHERS::RIGHT_TRIGGER:
		return GetRightTriggerValue() > 0.0f;

	default:
		return false;
	}
}

void KeyManager::MouseUpdate(void)
{
	Vector2I currentPos = {};
	GetMousePoint(
		&currentPos.x,
		&currentPos.y
	);

	Vector2I move = {};

	if (mouseFixed)
	{
		const Vector2I center = {
			Application::SCREEN_SIZE_X_HALF,
			Application::SCREEN_SIZE_Y_HALF
		};

		// 固定中は画面中央からの差分をそのままマウス移動量として使用する
		move = currentPos - center;

		mouseInfo.prev = center;
		mouseInfo.now = center;

		SetMousePoint(
			center.x,
			center.y
		);
	}
	else
	{
		mouseInfo.prev = mouseInfo.now;
		mouseInfo.now = currentPos;
		move = mouseInfo.now - mouseInfo.prev;
	}

	const Vector2 moveVector = move.ToVector2();

	if (moveVector.LengthSq() > MOUSE_THRESHOLD * MOUSE_THRESHOLD)
	{
		mouseInfo.moveSize = move;
		mouseInfo.moveNorm = moveVector.Normalized();

		// マウスが実際に動いた場合は、最後の入力デバイスをキーボード・マウス側へ更新する
		lastInputKinds = false;
	}
	else
	{
		mouseInfo.moveSize = Vector2I();
		mouseInfo.moveNorm = Vector2();
	}
}

bool KeyManager::GetControllerConnect(void) const
{
	return controllerConnected;
}

Vector2 KeyManager::GetRightStickVec(void) const
{
	if (!controllerConnected) { return Vector2(); }

	return ApplyStickDeadZone(
		controllerState.ThumbRX,
		controllerState.ThumbRY,
		RIGHT_STICK_DEAD_ZONE
	);
}

Vector2 KeyManager::GetLeftStickVec(void) const
{
	if (!controllerConnected) { return Vector2(); }

	return ApplyStickDeadZone(
		controllerState.ThumbLX,
		controllerState.ThumbLY,
		LEFT_STICK_DEAD_ZONE
	);
}

Vector2 KeyManager::ApplyStickDeadZone(short x, short y, short deadZone)const
{
	// shortの負側だけ32768まであるため、各軸を安全に-1.0f～1.0fへ変換する
	const float normalizedX =
		(x >= 0) ?
		static_cast<float>(x) / 32767.0f :
		static_cast<float>(x) / 32768.0f;

	const float normalizedY =
		(y >= 0) ?
		static_cast<float>(y) / 32767.0f :
		static_cast<float>(y) / 32768.0f;

	Vector2 stick(
		normalizedX,
		normalizedY
	);

	const float length = stick.Length();

	const float normalizedDeadZone =
		static_cast<float>(deadZone) /
		32767.0f;

	// 円形デッドゾーン内なら完全な入力なしとして扱う
	if (length <= normalizedDeadZone)
	{
		return Vector2();
	}

	// 斜め入力で長さが1.0fを超える場合を丸める
	const float clampedLength =
		(std::min)(length, 1.0f);

	// デッドゾーンの外側を0.0f～1.0fへ再割り当てする
	const float magnitude =
		(clampedLength - normalizedDeadZone) /
		(1.0f - normalizedDeadZone);

	return stick.Normalized() * std::clamp(magnitude, 0.0f, 1.0f);
}

float KeyManager::GetLeftTriggerValue(void) const
{
	if (!controllerConnected) { return 0.0f; }

	if (controllerState.LeftTrigger <= TRIGGER_DEAD_ZONE)
	{
		return 0.0f;
	}

	return
		static_cast<float>(
			controllerState.LeftTrigger -
			TRIGGER_DEAD_ZONE
		)
		/
		static_cast<float>(
			255 -
			TRIGGER_DEAD_ZONE
		);
}

float KeyManager::GetRightTriggerValue(void) const
{
	if (!controllerConnected) { return 0.0f; }

	if (controllerState.RightTrigger <= TRIGGER_DEAD_ZONE)
	{
		return 0.0f;
	}

	return
		static_cast<float>(
			controllerState.RightTrigger -
			TRIGGER_DEAD_ZONE
		)
		/
		static_cast<float>(
			255 -
			TRIGGER_DEAD_ZONE
		);
}

void KeyManager::SetMouseFixed(bool fixed)
{
	mouseFixed = fixed;

	SetMouseDispFlag(
		!fixed
	);

	// モード切り替え直後に大きな差分が出ないよう、現在値と前回値を揃える
	if (mouseFixed)
	{
		const Vector2I center = {
			Application::SCREEN_SIZE_X_HALF,
			Application::SCREEN_SIZE_Y_HALF
		};

		SetMousePoint(
			center.x,
			center.y
		);

		mouseInfo.now = center;
		mouseInfo.prev = center;
	}
	else
	{
		GetMousePoint(
			&mouseInfo.now.x,
			&mouseInfo.now.y
		);

		mouseInfo.prev =
			mouseInfo.now;
	}

	mouseInfo.moveNorm = Vector2();
	mouseInfo.moveSize = Vector2I();
}