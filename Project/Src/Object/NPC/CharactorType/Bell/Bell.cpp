#include "Bell.h"

#include "../../../../Utility/Utility.h"

#include "../../../../Manager/Input/InputManager.h"
#include "../../../../Manager/Camera/CurrentCamera.h"

#include "../../../../Scene/Common/GameSpace/GameSpaceController.h"

#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/AttackCollider.h"

Bell::Bell()
	: CharacterBase("Data/Parameter/Npc/")
{
}

void Bell::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しによる重みを設定
	SetPushWeight(50);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("NPC/Bell");

	// モデルのスケール設定
	trans.scale = 2;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -102.81f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.localAngle = Vector3(0.0f, Deg2Rad(180.0f), 0.0f);

	AnimePlay((int)ANIME_TYPE::Idle);

#pragma endregion


#pragma region アニメーション読み込み

	// アニメーションコントローラーの生成
	CreateAnimationController();

	// アニメーションの読み込み
	AddInFbxAnimation((int)ANIME_TYPE::Max, ANIME_SPEED_TABLE, ANIME_LOOP_TABLE);

#pragma endregion


#pragma region コライダーの生成

	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::Npc,
			Vector3::Yonly(-45.0f) * trans.scale,
			Vector3::Yonly(-50.0f) * trans.scale,
			50.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion

}

void Bell::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Bell::SubInit(void)
{
	trans.localAngle.y = Deg2Rad(GetParameter("Init", "angle"));
	trans.pos = Vector3(GetParameter("Init", "pos"));
}

void Bell::SubUpdate(void)
{
}
