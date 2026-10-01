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

	/*ColliderCreate(
		new CapsuleCollider(COLLIDER_TAG::Icicle,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));*/

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
}

void Bamboo::SubUpdate(void)
{

}
