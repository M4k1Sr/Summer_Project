#include "CactusWait.h"
#include "../../../../Object/Player/Player.h"

CactusWait::CactusWait(const Vector3& cactusPos)
	: cactusPos(cactusPos)
{
	player_ = std::make_unique<Player>(
		Vector3(0.0f,0.0f,0.0f),
		false, 
		false, 
		false, 
		0);
}

void CactusWait::OwnStateConditionUpdate(void)
{
}

void CactusWait::Update(void)
{
	Vector3 playerPos = player_->GetPos();

	//プレイヤーとのベクトルを取得
	Vector3 vec = playerPos - cactusPos;
	//Y軸方向は移動しない
	vec.y = 0.0f;

	//距離が近い場合ステートを切り替え
	if (vec.Length() < 10.0f)
	{
		
	}


}
