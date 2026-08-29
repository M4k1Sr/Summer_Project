#pragma once
#include "../Common/CharacterBase/CharacterBase.h"

class Player :
	public CharacterBase
{
public:

	Player(

		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight,

		bool isOperator
	) :
		CharacterBase("Data/Parameter/Player/"),
		INIT_POS(pos),
		isOperator(isOperator)
	{
		trans.pos = INIT_POS;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}

	~Player()override = default;

	void Load(void) override;

private:

	// ëÄçÏâ¬î\Ç©Ç«Ç§Ç©
	bool isOperator;

	// èâä˙ç¿ïW
	const Vector3 INIT_POS;

	void CharacterInit(void) override;
	void CharacterUpdate(void) override;
	void CharacterDraw(void) override;
	void CharacterAlphaDraw(void) override;
	void CharacterUiDraw(void) override;
	void CharacterRelease(void) override;

	Vector3 GetMoveDirection(void) const;

	void ResetPos(void) { trans.pos = INIT_POS; }
};


