#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

class PlayerPunchState : public CharacterStateBase
{

public:


	PlayerPunchState(
		const Vector3& playerPos
	);
	~PlayerPunchState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 更新処理
	void Update(void);

private:

#pragma region 受け取る参照

	// プレイヤーの座標
	const Vector3& playerPos;



#pragma endregion

};

