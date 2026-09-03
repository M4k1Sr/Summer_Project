#include "Cactus.h"
#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/CactusWait.h"

void Cactus::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/Cactus/Cactus");

	ColliderCreate(new SphereCollider(COLLIDER_TAG::Enemy, 80.0f));
	// モデルの角度のズレを設定
	trans.localAngle.x = Deg2Rad(90.0f);
	trans.localAngle.y = Deg2Rad(90.0f);
	//サイズ設定
	trans.scale = 1.3f;

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	//待機状態追加
	AddState(
		STATE::Wait,
		new CactusWait(
			trans.pos
		));

#pragma endregion
}

void Cactus::CharacterInit(void)
{
	ChangeState(STATE::Wait);
}

void Cactus::CharacterUpdate(void)
{
}

void Cactus::CharacterDraw(void)
{
}

void Cactus::CharacterAlphaDraw(void)
{
}

void Cactus::CharacterUiDraw(void)
{
}

void Cactus::CharacterRelease(void)
{
}
