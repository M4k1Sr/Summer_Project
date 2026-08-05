#pragma once

#include "../SceneBase.h"
#include "../../Common/Vector2.h"

class TitleScene : public SceneBase
{
public:
	TitleScene(void);
	~TitleScene(void)override = default;

private:
	void SubPreInit(void)override;
	void SubPreLoad(void)override;
	void SubPreUpdate(void)override;
	void SubPostUpdate(void)override;
	void SubPostUiDraw(void)override;

private:

	//タイトルシーンの状態遷移
	enum class TitleState
	{
		Title = 5,		//タイトル画面
		Movie = 10,		//ロゴの後ろでゲームムービーを流す
		MoveToDemo,	//デモシーンへ遷移
	};

	static constexpr int TITLE_LOGO_WIDTH = 552;	//タイトルロゴの横幅
	static constexpr int TITLE_LOGO_HEIGHT = 106;	//タイトルロゴの縦幅
	static constexpr int TITLE_LOGO_WIDTH_HALF = 276;	//タイトルロゴの横幅の半分
	static constexpr int TITLE_LOGO_HEIGHT_HALF = 53;	//タイトルロゴの縦幅の半分

	//メンバ変数
	int titleLogoHandle_;		//タイトルロゴのハンドル
	TitleState state_;		//タイトルシーンの状態
	Vector2 pos_;		//タイトルロゴの座標
	float animTime_;	//タイトルロゴのアニメーション時間

};
