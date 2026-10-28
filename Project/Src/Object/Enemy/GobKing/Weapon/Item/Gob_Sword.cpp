#include "Gob_Sword.h"

#include "../../../../Common/Collider/CapsuleCollider.h"

#include "../../../../../Utility/Utility.h"

Gob_Sword::Gob_Sword(
	const Vector3& COLL_LOCAL_POS,
	const Transform& playerTrans
) :
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans)
{
}

void Gob_Sword::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(false);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Weapon/Gob_Sword");

	// 初期ローカル座標・角度
	swordLocalPos = INIT_LOCAL_POS;
	swordLocalRot = Vector3(Deg2Rad(INIT_LOCAL_ROT_X), Deg2Rad(INIT_LOCAL_ROT_Y), Deg2Rad(INIT_LOCAL_ROT_Z));

	// モデルのスケール設定
	trans.scale = SCALE;

	// モデルの中心点のズレの補正
	trans.centerDiff = CENTER_DIFF * trans.scale;

	// 武器の座標を手フレームに設定
	trans.pos = MV1GetFramePosition(trans.model, SWORD_NUMBER);

#pragma endregion

}

void Gob_Sword::SubUpdate(void)
{
	// プレイヤーモデルの手フレームのワールド行列（アニメ反映済み）
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(playerTrans.model, SWORD_NUMBER);

	// 剣のローカル変換（手に対するオフセット）
	MATRIX scaleMat = MGetScale(VGet(trans.scale.x, trans.scale.y, trans.scale.z));
	MATRIX rotMat = MGetRotX(swordLocalRot.x);     // 必要に応じてXYZ合成
	rotMat = MMult(rotMat, MGetRotY(swordLocalRot.y));
	rotMat = MMult(rotMat, MGetRotZ(swordLocalRot.z));
	MATRIX transMat = MGetTranslate(VGet(swordLocalPos.x, swordLocalPos.y, swordLocalPos.z));

	MATRIX localMat = MMult(MMult(scaleMat, rotMat), transMat);

	// 子(剣) × 親(手) の順で合成
	MATRIX mat = MMult(localMat, handMat);

	// 当たり判定用に位置を保存
	trans.pos = VTransform(VGet(0, 0, 0), mat);
}
