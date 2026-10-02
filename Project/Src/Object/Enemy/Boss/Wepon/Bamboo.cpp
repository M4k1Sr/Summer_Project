#include "Bamboo.h"
#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"
Bamboo::Bamboo(const Vector3*& playerPos)
	:ActorBase(),
	playerPos(playerPos)
{
}

void Bamboo::Load(void)
{
	// モデルをロード
	trans.LoadModel("Bamboo/Bamboo");

	ColliderCreate(
		new CapsuleCollider(COLLIDER_TAG::Icicle,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 1.3f;

	//初期ステート
	state = STATE::None;

	//描画オフ
	SetIsDraw(true);
	//判定オフ
	SetJudge(false);
}

void Bamboo::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Player:
		state = STATE::None;

		SetIsDraw(false);

		SetJudge(false);
		break;
	case COLLIDER_TAG::Enemy:
		break;
	case COLLIDER_TAG::Stage:
		SetIsDraw(false);

		SetJudge(false);
		break;
	case COLLIDER_TAG::Icicle:
		break;
	default:
		break;
	}
}

void Bamboo::SubUpdate(void)
{
	//状態管理
	switch (state)
	{
	case Bamboo::STATE::None: { return; }

	case Bamboo::STATE::Create:
	{
		SetGravityFlg(false);

		trans.scale += SCALE_POW;
		if (trans.scale.MaxElementF() >= MODEL_SCALE) {
			state = STATE::Wait;
		}
		break;
	}

	case Bamboo::STATE::Wait: {

		if (++waitCounter >= WAIT_TIME)
		{
			SetGravityFlg(true);

			state = STATE::Fall;
		}
		break;
	}

	case Bamboo::STATE::Fall:
	{
		//画面外にでたら消す
		if (trans.pos.y <= MAX_FALL_POS_Y)
		{
			state = STATE::None;

			SetIsDraw(false);
			SetJudge(false);
		}
		break;
	}

	}
}

void Bamboo::Start(void)
{
	// 描画判定を有効にする
	SetIsDraw(true);

	// 当たり判定を有効にする
	SetJudge(true);

	// スケールを0にする
	trans.scale = 0.0f;

	//加速度初期化
	velocity = 0.0f;

	// 重力を消す
	SetGravityFlg(false);

	// 座標を設定
	trans.pos = *playerPos + CREATE_LOCAL_POS;

	//カウンター初期化
	waitCounter = 0;

	// 状態を生成中にする
	state = STATE::Create;
}
