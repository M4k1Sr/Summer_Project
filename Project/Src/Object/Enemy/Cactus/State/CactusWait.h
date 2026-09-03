#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../Common/Vector3.h"
#include <memory>
class Player;

class CactusWait : public CharacterStateBase
{
public:

	CactusWait(
		const Vector3& cactusPos
	);
	~CactusWait()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;
	// 更新処理
	void Update(void);

private:


#pragma region 受け取る参照

	std::unique_ptr<Player> player_;

	//サボテン座標
	const Vector3& cactusPos;


#pragma endregion
};

