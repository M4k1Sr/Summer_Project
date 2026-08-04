#include "GamePause.h"

#include <DxLib.h>

#include "../../../Manager/Input/KeyManager.h"
#include "../../SceneManager/SceneManager.h"

GamePause::GamePause(void) : SceneBase()
{
}

void GamePause::SubPostUpdate(void)
{
	// 予約式なので、この関数内でPopSceneを呼んでもGamePauseはまだ破棄されない
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down)
	{
		SceneManager::GetIns().PopScene();
	}
}

void GamePause::SubPostUiDraw(void)
{
	DrawBox(0, 0, 1280, 720, 0x000000, false);
	DrawString(20, 20, "ポーズ", 0xffffff);
}
