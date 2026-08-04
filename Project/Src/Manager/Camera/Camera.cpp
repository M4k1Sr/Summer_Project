#include "Camera.h"

#include <cmath>
#include <algorithm>

#include "../../Utility/Utility.h"
#include "../../Application/Application.h"
#include "../input/KeyManager.h"

#include "../../Scene/Common/GameSpace/GameSpaceController.h"

Camera::Camera(void) :

	mode(MODE::None),

	modeFuncPtr(),
	modeApply(),

	pos(),
	angle(),
	fov(),

	ROT_POWER(0.0f),
	MOVE_POWER(0.0f),

	fixedLookAtPos(),
	lookAtDiff(),

	folowAt(nullptr),

	lookAtYangle(nullptr),
	lookTarget(nullptr),
	FOLLOW_AUTO_MIN_DISTANCE(),
	FOLLOW_AUTO_MAX_DISTANCE(),

	gameSpace(nullptr),
	gameSpaceFollowAt(nullptr),
	gameSpaceParameter(),
	cameraUp(0.0f, 1.0f, 0.0f)
{
}

void Camera::Init(void)
{
#pragma region モード別関数を格納
	SET_MODE_FUNC(MODE::None, &Camera::NonModeFunc);
	SET_APPLY(MODE::None, &Camera::NonModeFunc);

	SET_MODE_FUNC(MODE::FixedPoint, &Camera::FixedPointModeFunc);
	SET_APPLY(MODE::FixedPoint, &Camera::FixedPointApply);

	SET_MODE_FUNC(MODE::Free, &Camera::FreeModeFunc);
	SET_APPLY(MODE::Free, &Camera::FreeApply);

	SET_MODE_FUNC(MODE::LookAtFree, &Camera::LookAtFreeModeFunc);
	SET_APPLY(MODE::LookAtFree, &Camera::LookAtFreeAplly);

	SET_MODE_FUNC(MODE::Display, &Camera::DisplayModeFunc);
	SET_APPLY(MODE::Display, &Camera::DisplayAplly);

	SET_MODE_FUNC(MODE::FollowRemote, &Camera::FollowRemoteModeFunc);
	SET_APPLY(MODE::FollowRemote, &Camera::FollowRemoteApply);

	SET_MODE_FUNC(MODE::FollowAuto, &Camera::FollowAutoModeFunc);
	SET_APPLY(MODE::FollowAuto, &Camera::FollowAutoApply);

	SET_MODE_FUNC(MODE::GameSpaceFollow, &Camera::GameSpaceFollowModeFunc);
	SET_APPLY(MODE::GameSpaceFollow, &Camera::GameSpaceFollowApply);
#pragma endregion
}

void Camera::Update(void)
{
	// モード別関数の呼び出し
	(this->*modeFuncPtr[(int)mode])();

	Apply();
}

void Camera::Apply(void)
{
	// クリップ距離を設定する(ClearDrawScreenでリセットされる)
	SetCameraNearFar(VIEW_NEAR, VIEW_FAR);

	// 視野角を設定する(ClearDrawScreenでリセットされる)
	SetupCamera_Perspective(fov);

	// モード別関数の呼び出し
	(this->*modeApply[(int)mode])();
}

#pragma region 定点
void Camera::ChangeModeFixedPoint(const Vector3& pos, const Vector3& angle, float fov)
{
	// 現在の情報を破棄
	Release();

	// マウス設定
	Key::GetIns().SetMouseFixed(false);

	// 状態遷移
	mode = MODE::FixedPoint;

	// 座標を設定
	this->pos = pos;

	// 角度を設定
	this->angle = angle;

	// 視野角を設定
	this->fov = fov;
}

void Camera::FixedPointModeFunc(void)
{
}

void Camera::FixedPointApply(void)
{
	// 適用
	SetCameraPositionAndAngle(pos.ToVECTOR(), angle.x, angle.y, angle.z);
}
#pragma endregion

#pragma region フリー
void Camera::ChangeModeFree(float ROT_POWER, float MOVE_POWER, const Vector3& pos, const Vector3& angle, float fov)
{
	// 現在の情報を破棄
	Release();

	// マウス設定
	Key::GetIns().SetMouseFixed(true);

	// 状態遷移
	mode = MODE::Free;

	// 回転量
	this->ROT_POWER = ROT_POWER;

	// 移動量
	this->MOVE_POWER = MOVE_POWER;

	// 初期座標
	this->pos = pos;

	// 初期角度
	this->angle = angle;

	// 視野角を設定
	this->fov = fov;
}

void Camera::FreeModeFunc(void)
{
#pragma region 角度
	// 入力デバイスごとの特性を保った回転量を取得する
	// ・スティック：倒し量で速度が変わる
	// ・マウス　　：移動ピクセル数で回転量が変わる
	// ・キー　　　：一定速度
	angle += GetCameraRotationDelta(ROT_POWER);

	WrapYawAngle();
#pragma endregion

#pragma region 移動
	const Vector3 moveInput = GetFreeMoveInput();

	if (moveInput != 0.0f) {
		// 入力の大きさを維持したまま、カメラのY回転へ合わせて移動する
		pos += moveInput.TransMat(MGetRotY(angle.y)) * MOVE_POWER;
	}
#pragma endregion
}

void Camera::FreeApply(void)
{
	// 適用
	SetCameraPositionAndAngle(pos.ToVECTOR(), angle.x, angle.y, angle.z);
}
#pragma endregion

#pragma region 回転のみフリー（注視点起点）
void Camera::ChangeModeLookAtFree(const Vector3& fixedLookAtPos, const Vector3& lookAtDiff, float ROT_POWER, const Vector3& angle, float fov)
{
	// 現在の情報を破棄
	Release();

	// マウス設定
	Key::GetIns().SetMouseFixed(true);

	// 状態遷移
	mode = MODE::LookAtFree;

	// 注視点
	this->fixedLookAtPos = fixedLookAtPos;

	// 注視点からの相対座標
	this->lookAtDiff = lookAtDiff;

	// 回転量
	this->ROT_POWER = ROT_POWER;

	// 初期角度
	this->angle = angle;

	// 視野角を設定
	this->fov = fov;
}

void Camera::LookAtFreeModeFunc(void)
{
	angle += GetCameraRotationDelta(ROT_POWER);

	WrapYawAngle();

	ClampPitchAngle(Deg2Rad(-30.0f), Deg2Rad(60.0f));

	// 現在の注視点と角度から自身の座標を算出する
	pos = fixedLookAtPos + lookAtDiff.TransMat(MatrixAllMultXY({ Vector3::XYonly(angle.x,angle.y) }));
}

void Camera::LookAtFreeAplly(void)
{
	// 適用
	SetCameraPositionAndTarget_UpVecY(pos.ToVECTOR(), fixedLookAtPos.ToVECTOR());
}
#pragma endregion

#pragma region ディスプレイ（特定の座標を起点に周回しながら注視する）
void Camera::ChangeModeDisplay(const Vector3& fixedLookAtPos, const Vector3& lookAtDiff, float ROT_POWER, const Vector3& angle, float fov)
{
	// 現在の情報を破棄
	Release();

	// マウス設定
	Key::GetIns().SetMouseFixed(false);

	// 状態遷移
	mode = MODE::Display;

	// 注視点
	this->fixedLookAtPos = fixedLookAtPos;

	// 注視点からの相対座標
	this->lookAtDiff = lookAtDiff;

	// 回転量
	this->ROT_POWER = ROT_POWER;

	// 初期角度
	this->angle = angle;

	// 視野角を設定
	this->fov = fov;
}

void Camera::DisplayModeFunc(void)
{
	// 回転処理（設定された値横向きに回し続ける）
	angle += Vector3::Yonly(1.0f).Normalized() * ROT_POWER;

	if (angle.y >= Deg2Rad(360.0f)) { angle.y -= Deg2Rad(360.0f); }
	if (angle.y <= Deg2Rad(0.0f)) { angle.y += Deg2Rad(360.0f); }

	// 現在の追従対象の座標と角度情報から自身(カメラ)の座標を算出する
	pos = fixedLookAtPos + lookAtDiff.TransMat(MatrixAllMultXY({ Vector3::XYonly(angle.x,angle.y) }));
}

void Camera::DisplayAplly(void)
{
	// 適用
	SetCameraPositionAndTarget_UpVecY(pos.ToVECTOR(), fixedLookAtPos.ToVECTOR());
}
#pragma endregion

#pragma region 追従（手動操作）
void Camera::ChangeModeFollowRemote(const Vector3* folowAt, const Vector3& lookAt, const Vector3& lookAtDiff, float ROT_POWER, const Vector3& angle, float fov)
{
	// 現在の情報を破棄
	Release();

	// マウス設定
	Key::GetIns().SetMouseFixed(true);

	// 状態遷移
	mode = MODE::FollowRemote;

	// 追従対象を設定
	this->folowAt = folowAt;

	// 注視点対象を設定
	this->lookAt = lookAt;

	// 追従対象とのローカル座標を設定
	this->lookAtDiff = lookAtDiff;

	// 回転量をセット
	this->ROT_POWER = ROT_POWER;

	// 角度をセット
	this->angle = angle;

	// 座標をセット
	pos = (*folowAt + lookAt.TransMat(MatrixAllMultXY({ Vector3::Xonly(angle.x) }))) + lookAtDiff.TransMat(MatrixAllMultXY({ Vector3::Xonly(angle.x) }));

	// 視野角を設定
	this->fov = fov;
}

void Camera::FollowRemoteModeFunc(void)
{
	// 追従対象が設定されていなかったら処理をしない
	if (folowAt == nullptr) { return; }

	// このモードは横回転だけを受け付ける
	angle += GetCameraRotationDelta(ROT_POWER, false, true);

	WrapYawAngle();

	// 現在の追従対象の座標と角度情報からカメラ座標を算出する
	pos = *folowAt + lookAtDiff.TransMat(MatrixAllMultXY({ Vector3::XYonly(angle.x,angle.y) }));

	lookAtPos = *folowAt + lookAt.TransMat(MatrixAllMultXY({ Vector3::XYonly(angle.x,angle.y) }));
}

void Camera::FollowRemoteApply(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (folowAt == nullptr) { return; }

	// 適用
	SetCameraPositionAndTarget_UpVecY(pos.ToVECTOR(), lookAtPos.ToVECTOR());
}
#pragma endregion

#pragma region 追従（自動操作）
void Camera::ChangeModeFollowAuto(const Transform& folowAt, const Vector3* lookTarget, float FOLLOW_AUTO_MIN_DISTANCE, float FOLLOW_AUTO_MAX_DISTANCE, float fov)
{
	// 現在の情報を破棄
	Release();

	// マウス設定
	Key::GetIns().SetMouseFixed(false);

	// 状態遷移
	mode = MODE::FollowAuto;

	// 追従対象を設定
	this->folowAt = &folowAt.pos;

	// 追従対象の向き
	this->lookAtYangle = &folowAt.angle.y;

	// 視野に入れる対象物
	this->lookTarget = lookTarget;

	// 最低距離
	this->FOLLOW_AUTO_MIN_DISTANCE = FOLLOW_AUTO_MIN_DISTANCE;

	// 最大距離
	this->FOLLOW_AUTO_MAX_DISTANCE = FOLLOW_AUTO_MAX_DISTANCE;

	// 視野角を設定
	this->fov = fov;

#pragma region 座標
	// ２点間ベクトル
	Vector3 atToTarget = *this->folowAt - *this->lookTarget;

	// fovから必要距離を計算（縦fov基準）
	float needDist = (atToTarget.Length() * 0.5f) / tanf(fov * 0.5f);

	// 位置を算出
	pos = *this->folowAt + atToTarget.Normalized() * needDist;
	pos.y += (*this->folowAt - ((*this->folowAt + *this->lookTarget) * 0.5f)).Length();
#pragma endregion

#pragma region 角度
	angle = ((*this->folowAt + *this->lookTarget) * 0.5f) - pos;
	angle = Vector3::Yonly(atan2f(angle.x, angle.z));
#pragma endregion
}

void Camera::ChangeModeFollowAuto(const Vector3* folowAt, const float* lookAtYangle, const Vector3* lookTarget, float FOLLOW_AUTO_MIN_DISTANCE, float FOLLOW_AUTO_MAX_DISTANCE, float fov)
{
	// 現在の情報を破棄
	Release();

	// マウス設定
	Key::GetIns().SetMouseFixed(false);

	// 状態遷移
	mode = MODE::FollowAuto;

	// 追従対象を設定
	this->folowAt = folowAt;

	// 追従対象の向き
	this->lookAtYangle = lookAtYangle;

	// 視野に入れる対象物
	this->lookTarget = lookTarget;

	// 最低距離
	this->FOLLOW_AUTO_MIN_DISTANCE = FOLLOW_AUTO_MIN_DISTANCE;

	// 最大距離
	this->FOLLOW_AUTO_MAX_DISTANCE = FOLLOW_AUTO_MAX_DISTANCE;

	// 視野角を設定
	this->fov = fov;

#pragma region 座標
	// ２点間ベクトル
	Vector3 atToTarget = *this->folowAt - *this->lookTarget;

	// fovから必要距離を計算（縦fov基準）
	float needDist = (atToTarget.Length() * 0.5f) / tanf(fov * 0.5f);

	// 位置を算出
	pos = *this->folowAt + atToTarget.Normalized() * needDist;
	pos.y += (*this->folowAt - ((*this->folowAt + *this->lookTarget) * 0.5f)).Length();
#pragma endregion

#pragma region 角度
	angle = ((*this->folowAt + *this->lookTarget) * 0.5f) - pos;
	angle = Vector3::Yonly(atan2f(angle.x, angle.z));
#pragma endregion
}

void Camera::FollowAutoModeFunc(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (folowAt == nullptr || lookTarget == nullptr) { return; }

	// ２点間ベクトル
	Vector3 atToTarget = *folowAt - *lookTarget;

	// fovから必要距離を計算（縦fov基準）
	float needDist = std::clamp((atToTarget.Length() * 0.5f) / tanf(fov * 0.5f), FOLLOW_AUTO_MIN_DISTANCE, FOLLOW_AUTO_MAX_DISTANCE);

	// lookTargetからみてlookAtのそのさらに先にカメラをおきたいのでその方向を取得する
	Vector3 backDir = atToTarget.Normalized();

	// 目標カメラ位置
	Vector3 desiredPos = *folowAt + backDir * needDist;

	// 高さ補正
	desiredPos.y += std::clamp(((*folowAt - *lookTarget) * 0.5f).Length(), 250.0f, 400.0f);

	// 補間（ガタつき防止）
	const float smooth = 0.1f;
	pos += (desiredPos - pos) * smooth;
	if (pos.y <= CAMERA_DOWN) pos.y = CAMERA_DOWN;

	// 角度を算出する
	Vector3 center = (*folowAt + *lookTarget) * 0.5f;
	center.y = FOCUS_DOWN;
	angle = (center - pos);
	angle = Vector3::Yonly(atan2f(angle.x, angle.z));
}

void Camera::FollowAutoApply(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (folowAt == nullptr || lookTarget == nullptr) { return; }

	// 適用
	Vector3 center = (*folowAt + *lookTarget) * 0.5f;
	center.y = FOCUS_DOWN;
	SetCameraPositionAndTarget_UpVecY(pos.ToVECTOR(), center.ToVECTOR());
}
#pragma endregion


#pragma region ゲーム空間対応追従
void Camera::ChangeModeGameSpaceFollow(const Transform& followAt, const GameSpaceController& gameSpace, const GameSpaceCameraParameter& parameter)
{
	// 現在のモード固有参照を破棄する
	Release();

	// Free3D時だけ回転入力を使用するが、モード切り替えのたびにマウス固定を変更すると操作感が不安定になるため固定状態にする
	Key::GetIns().SetMouseFixed(true);

	// 状態遷移
	mode = MODE::GameSpaceFollow;

	// 参照先を登録する
	this->gameSpace = &gameSpace;
	gameSpaceFollowAt = &followAt;

	// パラメータを保存する
	gameSpaceParameter = parameter;
	gameSpaceParameter.smooth = std::clamp(gameSpaceParameter.smooth, 0.001f, 1.0f);
	fov = gameSpaceParameter.fov;

	// Free3Dへ切り替わった直後も対象の向きに近い角度から開始する
	angle = Vector3(0.0f, followAt.angle.y, 0.0f);

	// 初回だけは補間せず、正しい位置へ即座に配置する
	CalculateGameSpaceCamera(pos, lookAtPos, cameraUp);
}

void Camera::GameSpaceFollowModeFunc(void)
{
	// Scene切り替えや対象破棄後の安全処理
	if (gameSpace == nullptr || gameSpaceFollowAt == nullptr) { return; }

	Vector3 desiredPos = pos;
	Vector3 desiredLookAt = lookAtPos;
	Vector3 desiredUp = cameraUp;

	CalculateGameSpaceCamera(desiredPos, desiredLookAt, desiredUp);

	// Actorの細かな上下動やレール接線の切り替わりを直接カメラへ反映せず補間する
	const float smooth = gameSpaceParameter.smooth;
	pos += (desiredPos - pos) * smooth;
	lookAtPos += (desiredLookAt - lookAtPos) * smooth;
	cameraUp += (desiredUp - cameraUp) * smooth;

	if (cameraUp.LengthSq() <= 0.000001f) { cameraUp = Vector3::Yonly(1.0f); }
	else { cameraUp.Normalize(); }
}

void Camera::CalculateGameSpaceCamera(Vector3& desiredPos, Vector3& desiredLookAt, Vector3& desiredUp)
{
	if (gameSpace == nullptr || gameSpaceFollowAt == nullptr) { return; }

	const Vector3 targetPos = gameSpaceFollowAt->pos;
	const Vector3 velocity = gameSpaceFollowAt->Velocity();

	const MovementPlane& plane = gameSpace->GetPlane();

	Vector3 right = plane.right;
	Vector3 depth = plane.depth;
	Vector3 up = plane.up;

	if (right.LengthSq() <= 0.000001f) { right = Vector3::Xonly(1.0f); }
	else { right.Normalize(); }

	if (depth.LengthSq() <= 0.000001f) { depth = Vector3::Zonly(1.0f); }
	else { depth.Normalize(); }

	if (up.LengthSq() <= 0.000001f) { up = Vector3::Yonly(1.0f); }
	else { up.Normalize(); }

	desiredLookAt = targetPos + gameSpaceParameter.lookAtOffset;
	desiredUp = up;

	switch (gameSpace->GetMode()) {

	case GAME_SPACE_MODE::Side2D: {
		// 横方向の速度から少し先を映し、進行方向を見やすくする
		const float lookAhead = std::clamp(
			velocity.Dot(right) * 8.0f,
			-gameSpaceParameter.sideLookAhead,
			gameSpaceParameter.sideLookAhead
		);

		desiredLookAt += right * lookAhead;
		desiredPos = desiredLookAt - depth * gameSpaceParameter.sideDistance + up * gameSpaceParameter.sideHeight;
		break;
	}

	case GAME_SPACE_MODE::Plane3D: {
		// 奥行き移動を見渡せるよう、移動平面の正面から少し横へずらした固定視点にする
		desiredPos = desiredLookAt - depth * gameSpaceParameter.planeDistance + right * gameSpaceParameter.planeSideOffset + up * gameSpaceParameter.planeHeight;
		break;
	}

	case GAME_SPACE_MODE::Rail: {
		Vector3 tangent = gameSpace->GetRail().GetTangent(targetPos);
		if (tangent.LengthSq() <= 0.000001f) { tangent = right; }
		else { tangent.Normalize(); }

		// レール接線と上方向から、レールを横から見る方向を作る
		Vector3 side = tangent.Cross(up);
		if (side.LengthSq() <= 0.000001f) { side = depth; }
		else { side.Normalize(); }

		const float lookAhead = std::clamp(
			velocity.Dot(tangent) * 8.0f,
			-gameSpaceParameter.railLookAhead,
			gameSpaceParameter.railLookAhead
		);

		desiredLookAt += tangent * lookAhead;
		desiredPos = desiredLookAt - side * gameSpaceParameter.railDistance + up * gameSpaceParameter.railHeight;
		break;
	}

	case GAME_SPACE_MODE::Free3D: {
		// 完全3D時だけ手動回転入力を受け付ける
		angle += GetCameraRotationDelta(			gameSpaceParameter.freeRotPower		);

		WrapYawAngle();
		ClampPitchAngle(			Deg2Rad(-20.0f),			Deg2Rad(55.0f)		);

		const float horizontalDistance = std::cos(angle.x) * gameSpaceParameter.freeDistance;

		Vector3 backDirection(
			-std::sin(angle.y) * horizontalDistance,
			std::sin(angle.x) * gameSpaceParameter.freeDistance + gameSpaceParameter.freeHeight,
			-std::cos(angle.y) * horizontalDistance
		);

		desiredPos = desiredLookAt + backDirection;
		desiredUp = Vector3::Yonly(1.0f);
		break;
	}
	}
}

void Camera::GameSpaceFollowApply(void)
{
	if (gameSpace == nullptr || gameSpaceFollowAt == nullptr) { return; }

	// MovementPlane::upを使うことで、Y-up以外の移動平面にも対応できる
	SetCameraPositionAndTargetAndUpVec(
		pos.ToVECTOR(),
		lookAtPos.ToVECTOR(),
		cameraUp.ToVECTOR()
	);
}
#pragma endregion

Vector3 Camera::GetCameraRotationDelta(float stickRotPower, bool usePitch, bool useYaw)const
{
	const KeyManager& key = Key::GetIns();

	Vector3 result = Vector3();

#pragma region コントローラー
	// スティックは倒し量を保持したまま回転速度へ反映する
	const Vector2 stick = key.GetRightStickVec();

	if (stick != 0.0f) { result = stick.ToCameraRotation() * stickRotPower; }

#pragma endregion

#pragma region マウス
	// スティック入力がない場合、マウスの実移動ピクセル数を角度へ変換する
	if (result == 0.0f) {
		const Vector2 mouseMove = key.GetMouseMove();

		if (mouseMove != 0.0f) {
			result = mouseMove.ToVector2Screen().ToCameraRotation() * MOUSE_ROT_SENSITIVITY;
			result.x *= -1.0f;
		}
	}
#pragma endregion

#pragma region キーボード
	// アナログ入力がない場合だけ、キーによる一定速度回転を使用する
	if (result == 0.0f) {
		Vector2 keyInput = Vector2();

		if (key.GetInfo(KEY_TYPE::CAMERA_ROT_RIGHT).now) { keyInput.x += KEY_ROT_INPUT_POWER; }
		if (key.GetInfo(KEY_TYPE::CAMERA_ROT_LEFT).now) { keyInput.x -= KEY_ROT_INPUT_POWER; }
		if (key.GetInfo(KEY_TYPE::CAMERA_ROT_FRONT).now) { keyInput.y += KEY_ROT_INPUT_POWER; }
		if (key.GetInfo(KEY_TYPE::CAMERA_ROT_BACK).now) { keyInput.y -= KEY_ROT_INPUT_POWER; }

		if (keyInput.LengthSq() > 1.0f) { keyInput.Normalize(); }

		result = keyInput.ToCameraRotation() * stickRotPower;
	}
#pragma endregion

	if (!usePitch) { result.x = 0.0f; }
	if (!useYaw) { result.y = 0.0f; }

	// ロール回転は行わない
	result.z = 0.0f;

	return result;
}

Vector3 Camera::GetFreeMoveInput(void)const
{
	const KeyManager& key = Key::GetIns();

	// スティック入力は倒し量を維持する
	const Vector2 stick = key.GetLeftStickVec();

	Vector3 result = stick.ToVector3XZ();

	// スティック入力がない場合はキー入力を使う
	if (result == 0.0f) {

		if (key.GetInfo(KEY_TYPE::CAMERA_MOVE_FRONT).now) { result.z += 1.0f; }
		if (key.GetInfo(KEY_TYPE::CAMERA_MOVE_BACK).now) { result.z -= 1.0f; }
		if (key.GetInfo(KEY_TYPE::CAMERA_MOVE_RIGHT).now) { result.x += 1.0f; }
		if (key.GetInfo(KEY_TYPE::CAMERA_MOVE_LEFT).now) { result.x -= 1.0f; }

		// キーの斜め入力だけ速度が上がらないよう正規化する
		const Vector3 horizontal = Vector3::XZonly(result.x, result.z);

		if (horizontal.LengthSq() > 1.0f) {
			const Vector3 normalized = horizontal.Normalized();

			result.x = normalized.x;
			result.z = normalized.z;
		}
	}

	// 上下移動はデジタル入力として共通加算する
	if (key.GetInfo(KEY_TYPE::CAMERA_MOVE_UP).now) { result.y += 1.0f; }
	if (key.GetInfo(KEY_TYPE::CAMERA_MOVE_DOWN).now) { result.y -= 1.0f; }

	// 水平と上下を同時入力した際も全体速度が上がらないよう、最大長だけ制限する
	if (result.LengthSq() > 1.0f) { result.Normalize(); }

	return result;
}

void Camera::WrapYawAngle(void)
{
	const float oneRotation = Deg2Rad(360.0f);

	while (angle.y >= oneRotation) { angle.y -= oneRotation; }

	while (angle.y < 0.0f) { angle.y += oneRotation; }
}

void Camera::ClampPitchAngle(float minAngle, float maxAngle)
{
	angle.x = std::clamp(angle.x, minAngle, maxAngle);
}

void Camera::DrawDebug(void) const
{
	if (App::GetIns().IsDrawDebug()) {
		DrawFormatString(
			0, 70, 0xffffff,
			"カメラ座標　 ：(% .1f, % .1f, % .1f)",
			pos.x, pos.y, pos.z
		);
		DrawFormatString(
			0, 90, 0xffffff,
			"カメラ角度　 ：(% .1f, % .1f, % .1f)",
			Rad2Deg(angle.x),
			Rad2Deg(angle.y),
			Rad2Deg(angle.z)
		);
	}
}

void Camera::Release(void)
{
	switch (mode)
	{
	case Camera::MODE::None: { break; }

	case Camera::MODE::FixedPoint:
		break;
	case Camera::MODE::Free:
		break;
	case Camera::MODE::LookAtFree:
		break;
	case Camera::MODE::Display:
		break;
	case Camera::MODE::FollowRemote:
		folowAt = nullptr;

		lookAtDiff = {};
		ROT_POWER = 0.0f;
		break;
	case Camera::MODE::FollowAuto:
		folowAt = nullptr;
		lookAtYangle = nullptr;
		lookTarget = nullptr;

		break;

	case Camera::MODE::GameSpaceFollow:
		gameSpace = nullptr;
		gameSpaceFollowAt = nullptr;
		cameraUp = Vector3::Yonly(1.0f);
		break;
	}
}