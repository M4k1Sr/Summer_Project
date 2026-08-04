#pragma once

#include "../SceneBase.h"

class ClearScene : public SceneBase
{
public:
	ClearScene(void);
	~ClearScene(void)override = default;

private:
	void SubPostUpdate(void)override;
	void SubPostUiDraw(void)override;
};
