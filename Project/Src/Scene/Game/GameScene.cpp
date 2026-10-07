#include "GameScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"

#include "../../Manager/Camera/GameSpaceFollow/GameSpaceFollowCamera.h"

#include "../SceneManager.h"

#include "../ActorUseDefine.h"

#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/MeshDebugObject.h"

#include "../../Object/NPC/CharactorType/Bell/Bell.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/EnemyManager.h"

#include "../../Object/Stage/MainStage/FirstStage.h"

GameScene::GameScene() : WorldSceneBase()
{
}

void GameScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Game");

	// ベル
	ObjAdd(new Bell());

	// ステージ1
	ObjAdd(new FirstStage());

	// 操作対象（モデルあり）
	operatorObject = new Player();
	ObjAdd(operatorObject);

	//敵の生成
	ObjAdd(new EnemyManager);
	//プレイヤー座標渡し
	ActorSerch<EnemyManager>(actors)->SetPlayerPos(&operatorObject->GetTrans().pos);

}

void GameScene::SubWorldPostUpdate(void)
{
	//// ゲーム終了処理
	//if (Input::GetIns().GetInfo(KEY_TYPE::End).down) {
	//	SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	//}

	//// 決定
	//if (Input::GetIns().GetInfo(KEY_TYPE::Enter).down) {
	//	SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
	//}
}

void GameScene::SubUiDraw(void)
{
	DrawStringToHandle(0, 0, "ゲーム", 0xffffff, Font::GetIns().GetFont(FontKinds::Default45));
}

void GameScene::CreateCamera(void)
{
	Player* target = ActorSerch<Player>(actors);
	if (target == nullptr) { camera = nullptr; return; }
	camera = new GameSpaceFollowCamera(target->GetTrans(), GetGameSpace());
}