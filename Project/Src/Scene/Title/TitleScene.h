#pragma once

#include "../SceneBase.h"

class TitleScene : public SceneBase
{
public:
	TitleScene(void);
	~TitleScene(void)override = default;

private:
	void SubPostUpdate(void)override;
	void SubPostUiDraw(void)override;
};
