#pragma once
#include "../Common/CharacterBase/CharacterBase.h"
#include "../ColliderOperator/ColliderOperator.h"


class Player :
	public CharacterBase
{
public:

	Player();
	~Player()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

private:

	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,
		Move,
		Jump,

		Punch,

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,

		Walk,

		Run,

		Punch,

		Max
	};

	// アニメーション再生速度テーブル
	float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		0.5f,
		0.5f,
		0.5f,
		0.75f,
	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,
		true,
		true,
		false,
	};

#pragma endregion

	// 初期化処理
	void SubInit(void)override;

	// 更新処理
	void SubUpdate(void)override;
};


