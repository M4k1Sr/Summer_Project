#pragma once

#include <DxLib.h>

#include <string>
#include <vector>

#include "../../Common/Vector2.h"

class KeyManager
{
private:

	// コンストラクタ / デストラクタ
	KeyManager(void);
	~KeyManager(void) = default;

	// コピー・ムーブ操作を禁止
	KeyManager(const KeyManager&) = delete;
	KeyManager& operator=(const KeyManager&) = delete;
	KeyManager(KeyManager&&) = delete;
	KeyManager& operator=(KeyManager&&) = delete;

	static KeyManager* ins;

public:

#pragma region シングルトン定義
	// 生成 / 初期化処理
	static void CreateIns(void)
	{
		if (ins == nullptr)
		{
			ins = new KeyManager();
			ins->Init();
		}
	}

	// 取得
	static KeyManager& GetIns(void)
	{
		return *ins;
	}

	// 終了処理 / 削除
	static void DeleteIns(void)
	{
		if (ins != nullptr)
		{
			ins->Release();
			delete ins;
			ins = nullptr;
		}
	}
#pragma endregion

	// 更新
	void Update(void);

	enum class KEY_TYPE
	{
		NON = -1,

#pragma region プレイヤー操作
		// 移動
		PLAYER_MOVE_UP,
		PLAYER_MOVE_DOWN,
		PLAYER_MOVE_RIGHT,
		PLAYER_MOVE_LEFT,

		// ダッシュ
		PLAYER_DASH,

		// ジャンプ
		PLAYER_JUMP,
#pragma endregion

#pragma region カメラ操作
		// 移動
		CAMERA_MOVE_FRONT,
		CAMERA_MOVE_BACK,
		CAMERA_MOVE_RIGHT,
		CAMERA_MOVE_LEFT,
		CAMERA_MOVE_UP,
		CAMERA_MOVE_DOWN,

		// 回転
		CAMERA_ROT_FRONT,
		CAMERA_ROT_BACK,
		CAMERA_ROT_RIGHT,
		CAMERA_ROT_LEFT,
#pragma endregion

#pragma region システム操作
		// 汎用（ポーズ / 終了）キー
		PAUSE,

		// 汎用決定キー
		ENTER,

		// 汎用方向キー
		UP,
		DOWN,
		LEFT,
		RIGHT,

		// テキスト入力時の日本語 / 英語切り替えキー
		TEXT_INPUT_LANGUAGE_SWITCH,
#pragma endregion

#pragma region デバッグキー
		// デバッグオブジェクト操作
		DEBUG_OBJECT_FRONT,
		DEBUG_OBJECT_BACK,
		DEBUG_OBJECT_RIGHT,
		DEBUG_OBJECT_LEFT,
		DEBUG_OBJECT_UP,
		DEBUG_OBJECT_DOWN,

		// デバッグ描画スイッチ
		DEBUG_DRAW_SWITCH,

		// デバッグシーン切り替え
		DEBUG_MODE_SWITCH,

		// デバッグモード時、元シーンも更新する
		DEBUG_MODE_TOPUPDATE,

		// シーン再ロード
		DEBUG_RELOAD,
#pragma endregion

		MAX,
	};

	// ボタン入力情報
	struct KEY_INFO
	{
		bool prev = false;
		bool now = false;
		bool up = false;
		bool down = false;
	};

	// マウス入力情報
	struct MOUSE_INFO
	{
		Vector2I now = {};
		Vector2I prev = {};
		Vector2 moveNorm = {};
		Vector2I moveSize = {};
	};

#pragma region ゲット関数
	// 指定した論理入力の状態を取得
	const KEY_INFO GetInfo(KEY_TYPE key)const
	{
		return keyInfo[(int)key];
	}

	// コントローラー接続状態
	bool GetControllerConnect(void)const;

	// 左スティック入力を取得
	Vector2 GetLeftStickVec(void)const;

	// 右スティック入力を取得
	Vector2 GetRightStickVec(void)const;

	// 左トリガー入力を0～1で取得
	float GetLeftTriggerValue(void)const;

	// 右トリガー入力を0～1で取得
	float GetRightTriggerValue(void)const;

	// マウス入力情報
	const MOUSE_INFO GetMouseInfo(void)const
	{
		return mouseInfo;
	}

	// マウス移動方向
	const Vector2 GetMouseMoveNorm(void)const
	{
		return mouseInfo.moveNorm;
	}

	// マウス移動ピクセル数（整数）
	const Vector2I GetMouseMoveSize(void)const
	{
		return mouseInfo.moveSize;
	}

	// マウス移動ピクセル数
	Vector2 GetMouseMove(void)const
	{
		return mouseInfo.moveSize.ToVector2();
	}

	// 最後に使用した入力機器
	// true = コントローラー
	// false = キーボード / マウス
	bool LastInputKinds(void)const
	{
		return lastInputKinds;
	}
#pragma endregion

	/// <summary>
	/// マウスカーソルを画面中央へ固定する設定
	/// </summary>
	/// <param name="fixed">true = 固定する、false = 固定しない</param>
	void SetMouseFixed(bool fixed);

private:

#pragma region 初期化 / 解放
	void Init(void);
	void Release(void);
#pragma endregion

#pragma region コントローラー
	// スティックやトリガーなど、ボタン以外の入力種別
	enum class CONTROLLER_OTHERS
	{
		LEFTSTICK_UP,
		LEFTSTICK_DOWN,
		LEFTSTICK_RIGHT,
		LEFTSTICK_LEFT,

		RIGHTSTICK_UP,
		RIGHTSTICK_DOWN,
		RIGHTSTICK_RIGHT,
		RIGHTSTICK_LEFT,

		LEFT_TRIGGER,
		RIGHT_TRIGGER,
	};

	// XInput状態を1フレームに1回だけ更新する
	void ControllerUpdate(void);

	// スティック・トリガーなどの入力判定
	bool ControllerOthersInput(
		const CONTROLLER_OTHERS& input
	)const;

	// 円形デッドゾーンを適用する
	Vector2 ApplyStickDeadZone(
		short x,
		short y,
		short deadZone
	)const;

	// 左スティックのデッドゾーン
	static constexpr short LEFT_STICK_DEAD_ZONE = 10000;

	// 右スティックのデッドゾーン
	static constexpr short RIGHT_STICK_DEAD_ZONE = 10000;

	// トリガーのデッドゾーン
	static constexpr unsigned char TRIGGER_DEAD_ZONE = 30;

	// このフレームのXInput状態
	XINPUT_STATE controllerState;

	// コントローラー接続状態
	bool controllerConnected;
#pragma endregion

#pragma region ボタン入力
	void KeyUpdate(void);

	KEY_INFO keyInfo[(int)KEY_TYPE::MAX];

	std::vector<int>
		keyboardFormat[(int)KEY_TYPE::MAX];

	std::vector<int>
		controllerButtonFormat[(int)KEY_TYPE::MAX];

	std::vector<int>
		mouseButtonFormat[(int)KEY_TYPE::MAX];

	std::vector<CONTROLLER_OTHERS>
		controllerOthersFormat[(int)KEY_TYPE::MAX];
#pragma endregion

#pragma region マウス
	void MouseUpdate(void);

	MOUSE_INFO mouseInfo;

	bool mouseFixed;

	static constexpr float MOUSE_THRESHOLD = 0.0f;
#pragma endregion

	// 最後に使用した入力機器
	// true = コントローラー
	// false = キーボード / マウス
	bool lastInputKinds;
};

using Key = KeyManager;
using KEY_TYPE = Key::KEY_TYPE;
