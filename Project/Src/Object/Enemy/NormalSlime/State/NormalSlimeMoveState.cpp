#include "NormalSlimeMoveState.h"

NormalSlimeMoveState::NormalSlimeMoveState
(const Vector3& normalSlimePos, 
	const Vector3& initPos, 
	std::function<void(const Vector3& vec)> normalSlimeMoveAccel)
	:
	normalSlimePos(normalSlimePos),
	initPos(initPos),
	normalSlimeMoveAccel(normalSlimeMoveAccel)
{
    //初期座標を基準に、X軸方向の2点を往復する
    movePointList =
    {
        initPos + Vector3(MOVE_RANGE, 0.0f, 0.0f),
        initPos + Vector3(-MOVE_RANGE, 0.0f, 0.0f)
    };
}

void NormalSlimeMoveState::OwnStateConditionUpdate(void)
{
}

void NormalSlimeMoveState::Update(void)
{
    //巡回目標座標までのベクトルを取得
    Vector3 vec = movePointList[nowMovePoint] - normalSlimePos;
    //Y軸方向は移動しない
    vec.y = 0.0f;

    //巡回目標座標までの距離が近い場合
    if (vec.Length() < ARRIVE_DIST)
    {
        //巡回番号を更新(次の目標へ)
        nowMovePoint = (nowMovePoint + 1) % movePointList.size();
    }
    else
    {
        //巡回目標座標までのベクトルを正規化して加速移動する
        vec.Normalize();
        normalSlimeMoveAccel(vec);
    }

}
