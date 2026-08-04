#include "WorldSceneBase.h"

#include "../../../Object/ActorBase.h"
#include "../../../Manager/Camera/Camera.h"

WorldSceneBase::WorldSceneBase(void) :
	SceneBase(),
	gameSpace()
{
}

void WorldSceneBase::SubPreUpdate(void)
{
	// オブジェクトが入力を移動方向へ変換する前に、空間切り替え状態を進める
	gameSpace.Update();

	SubWorldPreUpdate();
}

void WorldSceneBase::SubPostUpdate(void)
{
	// 当たり判定の押し出し後に補正することで、2D中にZ方向へずれる問題を防ぐ
	ForEachObject([this](ActorBase& object) { gameSpace.ApplyConstraint(object); });

	SubWorldPostUpdate();
}

void WorldSceneBase::SubPostObjectAdd(ActorBase& object)
{
	// WorldSceneBaseが所有する全オブジェクトへ空間管理クラスを自動登録する
	object.SetGameSpaceController(&gameSpace);
}

void WorldSceneBase::SubPostAlphaDraw(void)
{
	gameSpace.DrawDebug();
	SubWorldDebugDraw();
}

void WorldSceneBase::ChangeCameraModeGameSpaceFollow(const Transform& followAt, const GameSpaceCameraParameter& parameter)
{
	Camera* camera = GetCamera();
	if (camera == nullptr) { return; }

	camera->ChangeModeGameSpaceFollow(followAt, gameSpace, parameter);
}
