#pragma once

#include "../Common/GameSpace/WorldSceneBase.h"

class GameScene : public WorldSceneBase
{
public:
	GameScene(void);
	~GameScene(void)override = default;

private:
	void SubWorldPostUpdate(void)override;
	void SubPostUiDraw(void)override;
};
