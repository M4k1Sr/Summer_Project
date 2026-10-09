#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../Common/Vector3.h"

class NormalSlimeMoveState 
	: public CharacterStateBase
{ 
public:

	NormalSlimeMoveState(
		const Vector3& normalSlimePos,
		const Vector3& initPos,
		std::function<void(const Vector3& vec)> normalSlimeMoveAccel
	);
	~NormalSlimeMoveState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;
	// 更新処理
	void Update(void);

private:

	//初期座標からの+最大距離
	static constexpr float MOVE_RANGE = 200;

	//到達判定距離
	static constexpr float ARRIVE_DIST = 20.0f;

	////現在の巡回番号
	//unsigned char nowMovePoint = 0;

	//現在の巡回番号
	size_t nowMovePoint = 0;

	//巡回座標リスト(initPosを基準に生成)
	std::vector<Vector3> movePointList;
	

#pragma region 受け取る参照

	//ノーマルスライム座標
	const Vector3& normalSlimePos;
	//初期生成座標
	const Vector3& initPos;

	// 加速移動関数
	std::function<void(const Vector3& vec)> normalSlimeMoveAccel;

#pragma endregion
};

