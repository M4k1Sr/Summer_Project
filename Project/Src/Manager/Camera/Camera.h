#pragma once

#include"../../Object/Common/Transform.h"

class GameSpaceController;

/// <summary>
/// GameSpaceControllerに追従するカメラの共通設定。
/// 空間モードごとの距離・高さをまとめて指定する。
/// </summary>
struct GameSpaceCameraParameter
{
	// 注視点へ加える高さ等の相対座標
	Vector3 lookAtOffset;

	// Side2D時の距離 / 高さ / 最大先読み距離
	float sideDistance;
	float sideHeight;
	float sideLookAhead;

	// Plane3D時の距離 / 高さ / 横ずらし量
	float planeDistance;
	float planeHeight;
	float planeSideOffset;

	// Rail時の距離 / 高さ / 最大先読み距離
	float railDistance;
	float railHeight;
	float railLookAhead;

	// Free3D時の距離 / 高さ / 回転速度
	float freeDistance;
	float freeHeight;
	float freeRotPower;

	// 位置と注視点の補間率（0より大きく1以下）
	float smooth;

	// 視野角
	float fov;

	GameSpaceCameraParameter(void) :
		lookAtOffset(0.0f, 100.0f, 0.0f),
		sideDistance(700.0f), sideHeight(170.0f), sideLookAhead(140.0f),
		planeDistance(650.0f), planeHeight(280.0f), planeSideOffset(180.0f),
		railDistance(700.0f), railHeight(220.0f), railLookAhead(120.0f),
		freeDistance(500.0f), freeHeight(130.0f), freeRotPower(3.0f * (DX_PI_F / 180.0f)),
		smooth(0.12f),
		fov(60.0f * (DX_PI_F / 180.0f))
	{
	}
};

class Camera
{
public:
	Camera(void);
	~Camera(void) = default;

	// カメラのクリップ範囲
	static constexpr float VIEW_NEAR = 10.0f;
	static constexpr float VIEW_FAR = 30000.0f;

	// カメラモード
	enum class MODE
	{
		// 未設定
		None,

		// 定点
		FixedPoint,

		// フリー
		Free,

		// 回転のみフリー（注視点起点）
		LookAtFree,

		// ディスプレイ（特定の座標を起点に周回しながら注視する）
		Display,

		// 追従（手動操作）
		FollowRemote,

		// 追従（自動操作）
		FollowAuto,

		// GameSpaceControllerの空間モードに合わせた追従
		GameSpaceFollow,

		Max
	};

#pragma region カメラモード切り替え
	// 定点モードに変更
	void ChangeModeFixedPoint(const Vector3& pos, const Vector3& angle, float fov = (60.0f * (DX_PI_F / 180.0f)));
	// フリーモードに変更
	void ChangeModeFree(float ROT_POWER, float MOVE_POWER, const Vector3& pos = Vector3(), const Vector3& angle = Vector3(), float fov = (60.0f * (DX_PI_F / 180.0f)));
	// 回転のみフリー（注視点起点）モードに変更
	void ChangeModeLookAtFree(const Vector3& fixedLookAtPos, const Vector3& lookAtDiff = Vector3::Zonly(-400), float ROT_POWER = 3.0f * (DX_PI_F / 180.0f), const Vector3& angle = Vector3(), float fov = 60.0f * (DX_PI_F / 180.0f));
	// ディスプレイ（特定の座標を起点に周回しながら注視する）モードに変更
	void ChangeModeDisplay(const Vector3& fixedLookAtPos, const Vector3& lookAtDiff = Vector3::Zonly(-400), float ROT_POWER = 0.5f * (DX_PI_F / 180.0f), const Vector3& angle = Vector3(), float fov = 60.0f * (DX_PI_F / 180.0f));
	// 追従（手動操作）モードに変更
	void ChangeModeFollowRemote(const Vector3* folowAt, const Vector3& lookAt = Vector3(), const Vector3& lookAtDiff = Vector3::Zonly(-400), float ROT_POWER = 3.0f * (DX_PI_F / 180.0f), const Vector3& angle = Vector3(), float fov = (60.0f * (DX_PI_F / 180.0f)));
	// 追従（自動操作）モードに変更（引数省略バージョン）
	void ChangeModeFollowAuto(const Transform& folowAt, const Vector3* lookTarget, float FOLLOW_AUTO_MIN_DISTANCE = 350.0f, float FOLLOW_AUTO_MAX_DISTANCE = 400.0f, float fov = (80.0f * (DX_PI_F / 180.0f)));
	// 追従（自動操作）モードに変更（引数非省略バージョン）
	void ChangeModeFollowAuto(const Vector3* folowAt,const float* lookAtYangle, const Vector3* lookTarget, float FOLLOW_AUTO_MIN_DISTANCE = 350.0f, float FOLLOW_AUTO_MAX_DISTANCE = 400.0f, float fov = (80.0f * (DX_PI_F / 180.0f)));

	// GameSpaceControllerの現在モードに合わせた追従へ変更
	void ChangeModeGameSpaceFollow(const Transform& followAt, const GameSpaceController& gameSpace, const GameSpaceCameraParameter& parameter = GameSpaceCameraParameter());
#pragma endregion

	// 更新
	void Update(void);

	// 描画前のカメラ設定
	void Apply(void);

	// デバッグ用描画
	void DrawDebug(void) const;

#pragma region ゲット関数
	// 座標
	const Vector3& GetPos(void)const { return pos; }

	// 角度
	const Vector3& GetAngle(void)const { return angle; }

	// 現在の注視点
	const Vector3& GetLookAtPos(void)const { return lookAtPos; }

	// 現在のカメラモード
	MODE GetMode(void)const { return mode; }
#pragma endregion

#pragma region セット関数
	// 座標を設定（追従モード時を除く。追従モード = FOLLOW_REMOTE / FOLLOW_AUTO）
	void SetPos(const Vector3& pos) {
		// 追従状態のときは処理をしない
		if (mode == MODE::FollowRemote || mode == MODE::FollowAuto) { return; }
		// 指定の座標を代入
		this->pos = pos; 
	}

	// 角度を設定（追従(自動操作)モード時を除く。追従(自動操作)モード = FOLLOW_AUTO）
	void SetAngle(const Vector3& angle) { 
		// 追従（自動操作）のときは処理をしない
		if (mode == MODE::FollowAuto) { return; }
		// 指定の角度を代入
		this->angle = angle;
	}

	// 追従モードのときの追従対象を途中で変更する。追従モード = FOLLOW_REMOTE / FOLLOW_AUTO
	void FollowToLookAtChange(const Transform& trans) {
		if (mode != MODE::FollowRemote && mode != MODE::FollowAuto) { return; }
		folowAt = &trans.pos;
		if (mode == MODE::FollowAuto) { lookAtYangle = &trans.angle.y; }
	}

	// 追従(自動操作)モードのときの視野に含める対象物を途中で変更する。追従(自動操作)モード = FOLLOW_AUTO
	void FollowAutoToLookTargetChange(const Vector3* lookTarget) { if (mode == MODE::FollowAuto) { this->lookTarget = lookTarget; } }
#pragma endregion

	// 初期化
	void Init(void);
	// 解放
	void Release(void);

private:

#pragma region 入力共通処理

	// マウス1ピクセルあたりの回転量
	static constexpr float MOUSE_ROT_SENSITIVITY =
		0.12f * (DX_PI_F / 180.0f);

	// キーボード入力をベクトルへ変換する際の入力値
	static constexpr float KEY_ROT_INPUT_POWER = 1.0f;

	/// <summary>
	/// カメラ回転入力を取得する。
	/// 右スティックは倒し量、マウスは移動ピクセル数、
	/// キーボードは一定入力として回転量へ変換する。
	/// </summary>
	/// <param name="stickRotPower">スティック・キーボードの最大回転量</param>
	/// <param name="usePitch">X軸回転を使用するか</param>
	/// <param name="useYaw">Y軸回転を使用するか</param>
	/// <returns>このフレームで加算する回転量</returns>
	Vector3 GetCameraRotationDelta(
		float stickRotPower,
		bool usePitch = true,
		bool useYaw = true
	)const;

	/// <summary>
	/// フリーカメラ用の移動入力を取得する。
	/// 左スティックは倒し量を維持し、
	/// キーボード入力は斜め方向で速度が増えないよう補正する。
	/// </summary>
	/// <returns>長さ0～1の移動入力</returns>
	Vector3 GetFreeMoveInput(void)const;

	// Y軸角度を0～360度の範囲へ収める
	void WrapYawAngle(void);

	// X軸角度を指定範囲へ制限する
	void ClampPitchAngle(
		float minAngle,
		float maxAngle
	);

#pragma endregion


	// モード
	MODE mode;

	using STATEFUNC = void (Camera::*)(void);
	STATEFUNC modeFuncPtr[(int)MODE::Max];
	STATEFUNC modeApply[(int)MODE::Max];
#define SET_MODE_FUNC(mode, func) {modeFuncPtr[(int)(mode)] = static_cast<STATEFUNC>(func);}
#define SET_APPLY(mode, func) {modeApply[(int)(mode)] = static_cast<STATEFUNC>(func);}

	// 未設定用
	void NonModeFunc(void) {}

#pragma region 全状態共通
	// カメラの位置
	Vector3 pos;

	// カメラの角度
	Vector3 angle;

	// 視野角
	float fov;

#pragma endregion

#pragma region FIXED_POINT
	// 更新処理
	void FixedPointModeFunc(void);

	// 適用
	void FixedPointApply(void);
#pragma endregion

#pragma region FREE
	// 更新処理
	void FreeModeFunc(void);

	// 回転量
	float ROT_POWER;

	// 移動量
	float MOVE_POWER;

	// 適用
	void FreeApply(void);
#pragma endregion

#pragma region LOOK_AT_FREE
	// 更新処理
	void LookAtFreeModeFunc(void);

	// 注視点
	Vector3 fixedLookAtPos;

	// 注視点からのローカル座標
	Vector3 lookAtDiff;

	// 回転量
	//float ROT_POWER;
	// ↑FREEのものを流用

	// 適用
	void LookAtFreeAplly(void);
#pragma endregion

#pragma region DISPLAY
	// 更新処理
	void DisplayModeFunc(void);

	// 注視点
	//Vector3 fixedLookAtPos;
	// ↑LOOK_AT_FREEのものを流用

	// 注視点からのローカル座標
	//Vector3 lookAtDiff;
	// ↑LOOK_AT_FREEのものを流用

	// 回転量
	//float ROT_POWER;
	// ↑FREEのものを流用
	
	// 適用
	void DisplayAplly(void);
#pragma endregion

#pragma region FOLLOW_REMOTE
	// 更新処理
	void FollowRemoteModeFunc(void);

	// 追従対象
	const Vector3* folowAt;

	// 追従対象からの相対注視点対象
	Vector3 lookAt;

	// 注視点座標
	Vector3 lookAtPos;

	// 追従対象からのローカル座標
	//Vector3 lookAtDiff;
	// ↑LOOK_AT_FREEのものを流用

	// 回転量
	//float ROT_POWER;
	// ↑FREEのものを流用

	// 適用
	void FollowRemoteApply(void);
#pragma endregion

#pragma region FOLLOW_AUTO

#pragma region 定数定義

	// カメラ最低地上高
	static constexpr float CAMERA_DOWN = 85.0f;

	// 注視点最低地上高
	static constexpr float FOCUS_DOWN = 70.0f;

#pragma endregion

	// 更新処理
	void FollowAutoModeFunc(void);

	// 追従対象
	//const Vector3* lookAt_;
	// ↑FOLLOW_REMOTEのものを流用

	// 追従対象の向き
	const float* lookAtYangle;

	// 視野に入れる対象物
	const Vector3* lookTarget;

	// 最低距離
	float FOLLOW_AUTO_MIN_DISTANCE;

	// 最大距離
	float FOLLOW_AUTO_MAX_DISTANCE;

	// 適用
	void FollowAutoApply(void);
#pragma endregion

#pragma region GAME_SPACE_FOLLOW
	// 更新処理
	void GameSpaceFollowModeFunc(void);

	// 適用
	void GameSpaceFollowApply(void);

	// 現在の空間モードに応じた目標座標・注視点・上方向を計算する
	void CalculateGameSpaceCamera(Vector3& desiredPos, Vector3& desiredLookAt, Vector3& desiredUp);

	// GameSpaceController（所有権は持たない）
	const GameSpaceController* gameSpace;

	// 追従対象Transform（所有権は持たない）
	const Transform* gameSpaceFollowAt;

	// 空間対応カメラ設定
	GameSpaceCameraParameter gameSpaceParameter;

	// 任意平面の上方向へ対応するため、適用時に使用する上方向
	Vector3 cameraUp;
#pragma endregion
};
