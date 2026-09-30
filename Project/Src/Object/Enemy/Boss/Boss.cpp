#include "Boss.h"
#include "../../../Utility//Utility.h"
#include "../../Common/Collider/CapsuleCollider.h"

void Boss::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/Boss/Boss");

	//ColliderCreate(new SphereCollider(COLLIDER_TAG::Enemy, 80.0f));

	//サイズ設定
	trans.scale = 3.0f;
	//アニメーションずれ修正
	trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;

	SetGravityFlg(false);

	for (ActorBase* subObject : subObjects) { subObject->Load(); }
}

void Boss::SubInit(void)
{
	for (ActorBase* subObject : subObjects) { subObject->Init(); }
}

void Boss::SubUpdate(void)
{
	for (ActorBase* subObject : subObjects) { subObject->Update(); }
}

void Boss::SubDraw(void)
{
	for (ActorBase* subObject : subObjects) { subObject->Draw(); }
}

void Boss::SubRelease(void)
{
	// 抱える下位アクター全ての解放処理
	for (ActorBase*& subObject : subObjects) {
		subObject->Release();
		delete subObject;
		subObject = nullptr;
	}
	subObjects.clear();
}
