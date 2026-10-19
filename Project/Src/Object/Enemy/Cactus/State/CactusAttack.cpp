#include "CactusAttack.h"

#include "../Wepon/Panch.h"


CactusAttack::CactusAttack(
	const Vector3& cactusPos,
	const Vector3*& playerPos,
	std::function<void(void)> changeStateWait,
	std::function<void(void)> playAnimationAttack,
	Quaternion& cactusRotation, Panch& panch)
	:
	cactusPos(cactusPos),
	playerPos(playerPos),
	changeStateWait(changeStateWait),
	playAnimationAttack(playAnimationAttack),
	cactusRotation(cactusRotation),
	panch_(panch),
	attackCoolTime(0),
	isCoolTime(false)
{
}

void CactusAttack::Update(void)
{
	//プレイヤーとのベクトルを取得
	Vector3 vec = *playerPos - cactusPos;
	//Y軸方向は移動しない
	vec.y = 0.0f;

	//距離が遠くなったら場合ステートを切り替え
	if (vec.Length() > CHANGE_STATE_DISTANCE)
	{
		changeStateWait();
	}

	//目標角度
	float goalAngle_y = atan2f(vec.x, vec.z);

	// 目標角度のQuaternionを生成
	const Quaternion goalRotation = Quaternion::FromRotationY(goalAngle_y);

	// 最短方向に補間
	cactusRotation = Quaternion::Slerp(cactusRotation, goalRotation, 0.15f);
	
	//クールタイムがない場合
	if (!isCoolTime)
	{
		isCoolTime = true;
		attackCoolTime = 0;

	}
	else
	{
		//カウント増加
		attackCoolTime++;

		if (attackCoolTime >= ATTACK_COOLTIME_MAX)
		{

			//攻撃アニメーションの再生
			playAnimationAttack();
			//クールタイムリセット
			isCoolTime = false;

			panch_.Start();
		}
	}

}
