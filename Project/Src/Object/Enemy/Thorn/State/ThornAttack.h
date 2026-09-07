#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

class ThornAttack : public CharacterStateBase
{
public:

	ThornAttack(
		const Vector3& thornPos
	);
	~ThornAttack()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;
	// 更新処理
	void Update(void);

private:

#pragma region 受け取る参照

	//とげ座標
	const Vector3& thornPos;

#pragma endregion

};

