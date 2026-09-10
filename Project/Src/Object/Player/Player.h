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

	/// コライダーすべてを取得
	std::vector<ColliderBase*> GetCollider(void)const override;

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
		0.5f,
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

	// プレイヤーが抱える下位アクター格納配列
	std::vector<ActorBase*> subObjects;

	// 初期化処理
	void CharacterInit(void)override;

	// 更新処理
	void CharacterUpdate(void)override;

	// 描画処理
	void CharacterDraw(void)override {
		// 抱える下位アクター全ての描画処理
		for (ActorBase* subObject : subObjects) { subObject->Draw(); }
	}

	// 半透明描画処理
	void CharacterAlphaDraw(void)override {
		// 抱える下位アクター全ての描画処理
		for (ActorBase* subObject : subObjects) { subObject->AlphaDraw(); }
	}

	// 解放処理
	void CharacterRelease(void)override {
		// 抱える下位アクター全ての解放処理
		for (ActorBase*& subObject : subObjects) {
			subObject->Release();
			delete subObject;
			subObject = nullptr;
		}
		subObjects.clear();
	}


};


