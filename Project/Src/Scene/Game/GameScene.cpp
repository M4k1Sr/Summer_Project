#include "GameScene.h"

#include <DxLib.h>
#include <memory>

#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Input/KeyManager.h"
#include "../SceneManager/SceneManager.h"

#include "Pause/GamePause.h"

GameScene::GameScene(void) : WorldSceneBase()
{
}

void GameScene::SubWorldPostUpdate(void)
{
	// ポーズシーンを上へ積む
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down)
	{
		SceneManager::GetIns().PushScene(std::make_unique<GamePause>());
		return;
	}

	// 予約式なので、ここでChangeSceneを呼んでも現在のGameSceneはUpdate終了まで生存する
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down)
	{
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Clear);
	}

}

void GameScene::SubPostUiDraw(void)
{
	DrawStringToHandle(0, 0, "Game Scene", 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45));
}
