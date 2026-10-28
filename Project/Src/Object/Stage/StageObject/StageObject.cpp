#include "StageObject.h"

#include "../../../Utility/Utility.h"

#include "../../Common/Collider/BoxCollider.h"

StageObject::StageObject()
{
}

void StageObject::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(false);

	// 重力を有効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しによる重さ
	SetPushWeight(PUSH_WEIGHT);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Stage/StageObject/ChangeBlock");

	// モデルのスケール設定
	trans.scale = SCALE;

	// モデル座標
	trans.pos = Vector3(POS);

#pragma endregion

#pragma region コライダーの生成

	// ステージオブジェクト(スイッチ)
	AddCollider(new BoxCollider(COLLIDER_TAG::Stage, COLL_SIZE));

#pragma endregion

}
