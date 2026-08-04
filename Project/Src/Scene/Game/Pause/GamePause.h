#pragma once

#include "../../SceneBase.h"

class GamePause : public SceneBase
{
public:
	GamePause(void);
	~GamePause(void)override = default;

	bool IsUseLoadingScreen(void)const override { return false; }

private:
	void SubPostUpdate(void)override;
	void SubPostUiDraw(void)override;
};
