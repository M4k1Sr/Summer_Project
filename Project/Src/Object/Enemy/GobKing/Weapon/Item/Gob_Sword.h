#pragma once

#include "../../../../Common/ActorBase/ActorBase.h"

class Gob_Sword
	: public ActorBase
{
public:

	// コンストラクタ
	Gob_Sword(const Vector3& COLL_LOCAL_POS,
		const Transform& playerTrans);

	// デストラクタ
	~Gob_Sword()override = default;

	// 読み込み
	void Load(void)override;

private:

	// 武器ローカル座標・角度
	Vector3 swordLocalPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 swordLocalRot = Vector3(0.0f, 0.0f, 0.0f);

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

#pragma region 定数

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// 武器ローカル座標・角度
	const Vector3 INIT_LOCAL_POS = VGet(9.5f, 8.5f, 10.5f);
	const float INIT_LOCAL_ROT_X = 31.0f;
	const float INIT_LOCAL_ROT_Y = 176.0f;
	const float INIT_LOCAL_ROT_Z = 130.0f;

	// モデルスケール
	const float SCALE = 1.0f;

	// モデルズレ修正
	const Vector3 CENTER_DIFF = VGet(0.0f, -60.0f, 0.0f);

	// モデルのボーン番号
	const int SWORD_NUMBER = 23;

#pragma endregion

	// 更新
	void SubUpdate(void)override;

};

