#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"
class NormalSlime : 
	public CharacterBase
{

public :

	enum class STATE
	{
		None = -1,

		//移動状態
		Move,
		//ダメージHIT
		Hit,
		//死亡時
		Die,

		Max
	};

	//アニメーション
	enum class ANIM
	{
		//待機
		Idle,
		//攻撃
		Attack,
		//移動
		Move,
		//HIT
		Hit,
		//死亡
		Die,

		MAX

	};

	NormalSlime(
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

#pragma region 定数

	//コライダーサイズ
	static constexpr float COLL_SIZE = 50.0f;
	//初期調整角度
	static constexpr float ADJUSTMENT_ANGLE = 180.0f;
	//初期調整角度
	static constexpr float ADJUSTMENT_SIZE = 1.3f;

	//スライム加速最大スピード
	const float SLIME_ACCEL_MAX = 7.0f;

#pragma endregion


	// 初期座標
	const Vector3 INIT_POS;

	//アニメーションスピード
	const float animationSpeedTabel_[(int)ANIM::MAX]
		= { 1.0f,1.0f,1.0f,1.0f,1.0f };

	void CharacterInit(void) override;
	void CharacterUpdate(void) override;
	void CharacterDraw(void) override;
	void CharacterAlphaDraw(void) override;
	void CharacterUiDraw(void) override;
	void CharacterRelease(void) override;

	void ResetPos(void) { trans.pos = INIT_POS; }
};

