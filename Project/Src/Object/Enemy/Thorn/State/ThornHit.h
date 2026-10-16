#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../Common/Vector3.h"

class ThornHit
	: public CharacterStateBase
{
public:

	ThornHit(
		const Vector3& CactusPos,
		std::function<void(void)> playAnimationHit
	);
	~ThornHit()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;
	// 更新処理
	void Update(void);

private:

#pragma region 受け取る参照

	//座標
	const Vector3& ThornPos;

	//アニメーション再生
	std::function<void(void)> playAnimationHit;

#pragma endregion
};

