#pragma once
#include "../Common/CharacterBase/CharacterBase.h"
#include "../ColliderOperator/ColliderOperator.h"


class Player :
	public CharacterBase
{
public:

	Player(
		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight
	) :
		CharacterBase("Data/Parameter/Player/"),
		INIT_POS(pos),
		attackOperator(std::bind(&Player::ColliderCreate, this, std::placeholders::_1))
	{
		trans.pos = INIT_POS;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}

	~Player()override = default;

	void Load(void) override;

	// プレイヤーの状態
	enum class STATE {
		None = -1,

		// 移動状態
		Move,
		
		//攻撃状態
		Punch,	//パンチ攻撃

		Max
	};

private:

	// 初期座標
	const Vector3 INIT_POS;

	//キャラクターUI画像
	int SelectCopyStock;
	int CopyStock;
	int SelectImage;

	//攻撃コライダーのオペレーター
	ColliderOperator attackOperator;


	void CharacterInit(void) override;
	void CharacterUpdate(void) override;
	void CharacterDraw(void) override;
	void CharacterAlphaDraw(void) override;
	void CharacterUiDraw(void) override;
	void CharacterRelease(void) override;

	Vector3 GetMoveDirection(void) const;

	void ResetPos(void) { trans.pos = INIT_POS; }
};


