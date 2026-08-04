#include "ClearScene.h"

#include <DxLib.h>

#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Input/KeyManager.h"
#include "../SceneManager/SceneManager.h"

ClearScene::ClearScene(void) : SceneBase()
{
}

void ClearScene::SubPostUpdate(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down)
	{
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	}

}

void ClearScene::SubPostUiDraw(void)
{
	DrawStringToHandle(0, 0, "Clear Scene", 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45));
}
