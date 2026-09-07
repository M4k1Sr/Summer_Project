#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"
class Thorn
	:public CharacterBase
{

public:

	enum class STATE
	{
		None = -1,

		//攻撃状態
		Attack,

		Max
	};

	Thorn(
		const Vector3& pos
	) :
		CharacterBase(),
		INIT_POS(pos)
	{
		trans.pos = INIT_POS;
		SetGravityFlg(true);
	}

	void Load(void) override;

private:

	// 初期座標
	const Vector3 INIT_POS;

	// プレイヤーが抱える下位アクター格納配列
	std::vector<ActorBase*> subObjects;

	// 初期化処理
	void CharacterInit(void)override;
	// 更新処理
	void CharacterUpdate(void)override;
	// 描画処理
	void CharacterDraw(void)override;
	// 半透明描画処理
	void CharacterAlphaDraw(void)override;
	void CharacterUiDraw(void) override;
	// 解放処理
	void CharacterRelease(void)override;



	Vector3 GetMoveDirection(void) const;

	void ResetPos(void) { trans.pos = INIT_POS; }

};