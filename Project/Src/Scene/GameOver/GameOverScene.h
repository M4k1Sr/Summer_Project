#pragma once

#include "../SceneBase.h"

class GameOverScene : public SceneBase
{
public:
	GameOverScene(void);
	~GameOverScene(void)override = default;

private:
	void SubPostUpdate(void)override;
	void SubPostUiDraw(void)override;
};
