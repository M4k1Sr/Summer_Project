#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"
class Cactus : 
	public CharacterBase
{

public:

	enum class STATE
	{
		None = -1,

		//待機状態
		Wait,
		//攻撃
		Attack,
		//HIT
		Hit,
		//HP0
		Die,

		Max
	};

	//サボテンアニメーション
	enum class ANIM
	{
		//待機
		Wait,
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

	Cactus(
		const Vector3& pos
	) :
		CharacterBase(),
		initPos(pos),
		playerPos_(nullptr)
	{
		trans.pos = initPos;
		SetGravityFlg(true);
	}

	void Load(void) override;

	//座標渡し
	void SetPlayerPos(const Vector3* pos) { playerPos_ = pos; }

private:

#pragma region 定数

	//コライダーサイズ
	static constexpr float COLL_SIZE = 50.0f;

	static constexpr float HIT_BOX_SIZE = 80.0f;

#pragma endregion

	// 初期座標
	const Vector3 initPos;

	//アニメーションスピード
	const float animationSpeedTabel_[(int)ANIM::MAX] 
		= { 1.0f,0.5f,1.0f,1.0f,1.0f };

	void SubInit(void) override;
	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;


	void ResetPos(void) { trans.pos = initPos; }

	//座標参照
	const Vector3 * playerPos_;
};


