#include "TitleScene.h"

#include <DxLib.h>

#include "../../Application/Application.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Input/KeyManager.h"
#include "../SceneManager/SceneManager.h"
#include "../../Application/Application.h"

#include "../Test/TestScene.h"

TitleScene::TitleScene(void) : SceneBase()
{
	titleLogoHandle_ = -1;
	state_ = TitleState::Title;
}

void TitleScene::SubPreInit(void)
{
	pos_ = Vector2(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	animTime_ = float(TitleState::Title);

}

void TitleScene::SubPreLoad(void)
{
	titleLogoHandle_ = LoadGraph("Data/Image/Title/TitleLogo.png");
}

void TitleScene::SubPreUpdate(void)
{
	switch (state_)
	{
	case TitleState::Title:
		if (animTime_ <= 0.0f)
		{
			state_ = TitleState::Movie;
		}
		break;
	case TitleState::Movie:
	{
		//タイトルロゴの座標を右下に移動させる
		int targetX = App::SCREEN_SIZE_X - TITLE_LOGO_WIDTH_HALF;
		int targetY = App::SCREEN_SIZE_Y - TITLE_LOGO_HEIGHT_HALF;
		float dx = targetX - pos_.x;
		float dy = targetY - pos_.y;

		float length = std::sqrt(dx * dx + dy * dy);

		if (length <= 2.0f)
		{
			pos_.x = targetX;
			pos_.y = targetY;
		}
		else
		{
			pos_.x += dx / length*2.0f;
			pos_.y += dy / length*2.0f;
		}

		break;
	}
	case TitleState::MoveToDemo:
		break;
	default:
		break;
	}
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

	// タイトルロゴのアニメーション
	animTime_ -= 1.0f/60.0f;	//仮のやつ


}

void TitleScene::SubPostUiDraw(void)
{
	DrawStringToHandle(0, 0, "タイトル", 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_45));
	DrawStringToHandle(0, 45, "「-」キーでテストシーンに遷移", 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));

	//タイトルロゴ
	DrawRotaGraph(pos_.x, pos_.y,1.0f,0.0f, titleLogoHandle_, TRUE);

}
