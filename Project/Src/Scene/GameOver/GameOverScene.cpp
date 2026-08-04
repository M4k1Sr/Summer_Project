#include "GameOverScene.h"

#include <DxLib.h>

#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Input/KeyManager.h"
#include "../SceneManager/SceneManager.h"

GameOverScene::GameOverScene(void) : SceneBase()
{
}

void GameOverScene::SubPostUpdate(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down)
	{
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	}

}

void GameOverScene::SubPostUiDraw(void)
{
	DrawStringToHandle(0, 0, "GameOver Scene", 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45));
}
