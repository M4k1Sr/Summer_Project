#pragma once

#include "../../Common/CharacterBase/CharacterBase.h"

class GobKing : public CharacterBase
{
public:

	// コンストラクタ
	GobKing(
		const Vector3& pos,
		const int initHp
	) :
		CharacterBase(),
		playerPos(nullptr),
		hitReact(false),
		invincibleFrame(INVINCIBLE_FRAME_MAX)
	{
		isDie = false,
		isFlyEnd = false,
		hp = initHp,
		SetGravityFlg(true);
	}

	// デストラクタ
	~GobKing()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	//座標渡し
	void SetPlayerPos(const Vector3* pos) { playerPos = pos; }

private:

#pragma region 定数

	// 聴覚範囲
	const float HEAR_RANGE = 2000.0f;
	// 索敵範囲
	const float SEARCH_RANGE = 1500.0f;
	// 攻撃可能範囲
	const float ATTACK_RANGE = 300.0f;

	// 押し出しによる重み
	const int PUSH_WEIGHT = 50;

	// スケール
	const float SCALE = 10.0f;

	// ズレの修正(座標)
	const Vector3 CENTER_DIFF = VGet(0.0f, -90.0f, 0.0f);

	// ズレの修正(角度)
	const float ROT_DIFF = 180.0f;

	// 初期座標
	const Vector3 INIT_POS = VGet(6000.0f, -700.0f, 0.0f);

	// コライダー座標・半径
	const float COLL_START_POS = 30.0f;
	const float COLL_END_POS = 60.0f;
	const float RADIUS = 30.0f;

	//  ゴブリンキングの攻撃半径
	const float GOB_ATTACK_RADIUS = 100.0f;

	// ゴブリンキングの攻撃ローカル座標
	const Vector3 GOB_ATTACK_LOCAL_POS = VGet(0, -400, 300);

	// 移動速度(最大値）
	const float GOB_ACCEL_MAX = 5.0f;

	// ゴブリンキングの攻撃判定の時間
	const float GOB_COLL_START_TIME = 0.5f;
	const float GOB_COLL_END_TIME = 0.6f;

	// ダメージを受けた際の無敵猶予フレーム 
	static constexpr int INVINCIBLE_FRAME_MAX = 60;

#pragma endregion

	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,
		Move,

		Roar,

		Throw_Rock,
		Attack_Swing,

		React,

		Flying,
		Death,

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,

		Roar,

		Walk,

		Swing,
		Taunt,
		Throw,

		HitReact,

		Flying,

		Max
	};

	// アニメーション再生速度テーブル
	float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		0.5f,	// Idle

		0.5f,	// Roar

		0.5f,	// Walk

		0.5f,	// Swing
		0.5f,	// Taunt
		0.5f,	// Throw

		0.5f,	// React

		0.5f,	// Flying

	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		false,	// Roar

		true,	// Walk

		false,	// Swing
		false,	// Taunt
		false,	// Throw

		false,	// React

		false,	// Flying
	};

#pragma endregion

	// 初期化処理
	void SubInit(void)override {

		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}

	// 更新処理
	void SubUpdate(void)override;

#pragma region 受けとる参照

	//座標参照
	const Vector3* playerPos;

	// ダメージを受けたか判定
	bool hitReact;

	// 無敵猶予フレーム
	int invincibleFrame;

	// 死亡したか
	bool isDie;

	// 敵が吹っ飛び終わったか
	bool isFlyEnd;

#pragma endregion

};