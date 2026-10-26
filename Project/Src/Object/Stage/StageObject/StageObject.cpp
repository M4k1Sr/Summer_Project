#include "StageObject.h"

#include "../../../Utility/Utility.h"

#include "../../Common/Collider/BoxCollider.h"

//#include "../Common/Shader/DefaultShader.h"
//#include "../Common/Shader/RimLightShader.h"
//#include "../Common/Shader/WaterShader.h"

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
	SetPushWeight(100);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Stage/StageObject/ChangeBlock");

	// モデルのスケール設定
	trans.scale = 1.0f;

	trans.pos = Vector3(7000.0f,-950.0f,0.0f);

	//// モデルの中心点のズレの補正
	//trans.centerDiff = Vector3(0.0f, -688.0f, 0.0f) * trans.scale;

	//// モデルの角度のズレの補正
	//trans.localAngle = Vector3(0.0f, Deg2Rad(180.0f), 0.0f);

#pragma endregion

#pragma region コライダーの生成

	ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, Vector3(100, 100, 100)));

#pragma endregion

}

void StageObject::SubUpdate(void)
{
}