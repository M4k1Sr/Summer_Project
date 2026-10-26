#include "HP.h"

HP::HP(void)
	:
	ActorBase(),

	image(-1)
{
}

void HP::Load(void)
{
	image = LoadGraph("Data/Image/PlayerUI/Heart");
	SetDrawType(ACTOR_DRAW_TYPE::Ui);
}

void HP::SubDraw(void)
{
	DrawRotaGraph(0.0f, 0.0f, 1.0f, 0.0f, image, false);
}
