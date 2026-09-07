#include "Icicle.h"

#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"


Icicle::Icicle(const Vector3& playerPos) :
	ActorBase(),

	playerPos(playerPos)
{
}

void Icicle::Load(void)
{
	// モデルをロード
	trans.LoadModel("Icicle/Icicle");

	ColliderCreate(
		new CapsuleCollider(COLLIDER_TAG::Enemy,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));
	// モデルの角度のズレを設定
	//trans.localAngle.y = Deg2Rad(90.0f);
	//サイズ設定
	trans.scale = 1.3f;
}


void Icicle::SubUpdate(void)
{
	//状態管理
	switch (state)
	{
	case Icicle::STATE::None: { return; }

	case Icicle::STATE::Create: {

		trans.scale += 0.05f;
		if (trans.scale.MaxElementF() >= MODEL_SCALE) {
			state = STATE::Wait;
		}

		break;
	}

	case Icicle::STATE::Wait: {

		if(++waitCounter >= WAIT_TIME) {

			SetGravityFlg(true);

			state = STATE::Fall;
		}

		break;
	}

	case Icicle::STATE::Fall: {


		//プレイヤーに当たったら消す


		//落下中に何かしらにステージ・敵にあったら消す


		//画面外にでたら消す
		if (trans.pos.y <= -5000.0f) {
			state = STATE::None;

			SetIsDraw(false);

			SetJudge(false);
		}

		break;
	}

	}
}

void Icicle::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Icicle::Start(void)
{
	// 描画判定を有効にする
	SetIsDraw(true);

	// 当たり判定を有効にする
	SetJudge(true);

	// スケールを0にする
	trans.scale = 0.0f;

	// 重力を消す
	SetGravityFlg(false);

	// 座標を設定
	trans.pos = playerPos + CREATE_LOCAL_POS;

	//カウンター初期化
	waitCounter = 0;

	// 状態を生成中にする
	state = STATE::Create;
}
