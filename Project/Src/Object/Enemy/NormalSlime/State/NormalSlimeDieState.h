#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../Common/Vector3.h"

class NormalSlimeDieState
: public CharacterStateBase
{
public:

	NormalSlimeDieState(
		const Vector3& normalSlimePos,
		std::function<void(void)> playAnimationDie
	);
	~NormalSlimeDieState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;
	// 更新処理
	void Update(void);

private:

#pragma region 受け取る参照

	//ノーマルスライム座標
	const Vector3& normalSlimePos;

	//アニメーション再生
	std::function<void(void)> playAnimationDie;

#pragma endregion
};




