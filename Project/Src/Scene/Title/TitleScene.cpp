#include "TitleScene.h"

#include <DxLib.h>

#include "../../Application/Application.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Input/KeyManager.h"
#include "../SceneManager/SceneManager.h"

#include "../Test/TestScene.h"

TitleScene::TitleScene(void) : SceneBase()
{
}

void TitleScene::SubPostUpdate(void)
{
	// ゲーム終了
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down)
	{
		App::GetIns().GameEnd();
		return;
	}

	// フェード付きでゲームシーンへ遷移
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down)
	{
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Game);
	}


	if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_RELOAD).down) {
		SceneManager::GetIns().ChangeSceneFade(std::make_unique<TestScene>());
	}
}

void TitleScene::SubPostUiDraw(void)
{
	DrawStringToHandle(0, 0, "タイトル", 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45));
	DrawStringToHandle(0, 45, "「-」キーでテストシーンに遷移", 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
}
